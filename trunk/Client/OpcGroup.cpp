#include "StdAfx.h"
#include "OpcGroup.h"
#include "OpcItem.h"
#include "OpcException.h"

OpcGroup::OpcGroup(const CString & groupName, bool active, unsigned long reqUpdateRate_ms, unsigned long &revisedUpdateRate_ms, float deadBand, OpcServer &server):
name(groupName),
opcServer(server)
{
	USES_CONVERSION;
	WCHAR* wideName = T2OLE(groupName);

	HRESULT result = opcServer.GetServerInterface()->AddGroup(wideName, active, reqUpdateRate_ms, 0, 0, &deadBand,
		0, &groupHandle, &revisedUpdateRate_ms, IID_IOPCGroupStateMgt, (LPUNKNOWN*)&iStateManagement);
	if (FAILED(result))
	{
		throw OpcException("Failed to Add group");
	} 

	result = iStateManagement->QueryInterface(IID_IOPCSyncIO, (void**)&iSychIO);
	if (FAILED(result))
	{
		throw OpcException("Failed to get IID_IOPCSyncIO");
	}

	result = iStateManagement->QueryInterface(IID_IOPCAsyncIO2, (void**)&iAsych2IO);
	if (FAILED(result))
	{
		throw OpcException("Failed to get IID_IOPCAsyncIO2");
	}

	result = iStateManagement->QueryInterface(IID_IOPCItemMgt, (void**)&iItemManagement);
	if (FAILED(result))
	{
		throw OpcException("Failed to get IID_IOPCItemMgt");
	}
}

OpcGroup::~OpcGroup()
{
	opcServer.GetServerInterface()->RemoveGroup(groupHandle, FALSE);
}

OpcServer& OpcGroup::GetServer()
{
	return opcServer;
}

IAsynchDataCallback* OpcGroup::GetUsrAsynchHandler()
{
	return userAsynchCBHandler;
}

const CString& OpcGroup::GetName()
{
	return name;
}

CComPtr<IOPCItemMgt>& OpcGroup::GetItemManagementInterface()
{
	return iItemManagement;
}

CComPtr<IOPCAsyncIO2>& OpcGroup::GetAsych2IOInterface()
{
	return iAsych2IO;
}

CComPtr<IOPCSyncIO>& OpcGroup::GetSychIOInterface()
{
	return iSychIO;
}

void OpcGroup::ReadSync(CAtlArray<OpcItem *>& items, OpcItem_DataMap & opcData, OPCDATASOURCE source)
{
	OPCHANDLE *serverHandles = BuildServerHandleList(items);
	HRESULT *itemResult;
	OPCITEMSTATE *itemState;
	DWORD noItems = (DWORD)items.GetCount();

	HRESULT	result = iSychIO->Read(source, noItems, serverHandles, &itemState, &itemResult);
	if (FAILED(result))
	{
		delete []serverHandles;
		throw OpcException("Read failed");
	} 

	for (unsigned i = 0; i < noItems; i++)
	{
		OpcItem * item = (OpcItem *)itemState[i].hClient;
		OpcItemData * data = AsyncDataCallback::MakeOpcDataItem(itemState[i].vDataValue, itemState[i].wQuality, itemState[i].ftTimeStamp, itemResult[i]);
		OpcItem_DataMap::CPair* pair = opcData.Lookup(item);
		if (pair == NULL)
		{
			opcData.SetAt(item,data);
		} 
		else 
		{
			opcData.SetValueAt(pair,data);
		}
	}

	delete []serverHandles;
	OpcHost::ComFree(itemResult);
	OpcHost::ComFree(itemState);	
}

Transaction * OpcGroup::ReadAsync(CAtlArray<OpcItem *>& items, ITransactionComplete *transactionCB)
{
		DWORD cancelID;
		HRESULT * individualResults;
		Transaction * trans = new Transaction(items,transactionCB);
		OPCHANDLE *serverHandles = BuildServerHandleList(items);
		DWORD noItems = (DWORD)items.GetCount();

		HRESULT result = iAsych2IO->Read(noItems, serverHandles, (DWORD)trans, &cancelID, &individualResults);
		delete [] serverHandles;
		if (FAILED(result))
		{
			delete trans;
			throw OpcException("Asynch Read failed");
		}

		trans->SetCancelId(cancelID);
		unsigned failCount = 0;
		for (unsigned i = 0;i < noItems; i++)
		{
			if (FAILED(individualResults[i]))
			{
				trans->SetItemError(items[i],individualResults[i]);
				failCount++;
			}
		}
		if (failCount == items.GetCount())
		{
			trans->SetCompleted(); // if all items return error then no callback will occur.
		}
		
		OpcHost::ComFree(individualResults);
		return trans;
}

OpcItem * OpcGroup::AddItem(CString &itemName, bool active)
{
	CAtlArray<CString> names;
	CAtlArray<OpcItem *> itemsCreated;
	CAtlArray<HRESULT> errors;
	names.Add(itemName);
	if (AddItems(names, itemsCreated, errors, active)!= 0)
	{
		throw OpcException("Failed to add item");
	}
	return itemsCreated[0];
}

int OpcGroup::AddItems(CAtlArray<CString>& itemName, CAtlArray<OpcItem *>& itemsCreated, CAtlArray<HRESULT>& errors, bool active)
{
	itemsCreated.SetCount(itemName.GetCount());
	errors.SetCount(itemName.GetCount());
 	OPCITEMDEF *itemDef = new OPCITEMDEF[itemName.GetCount()];
	
	for (unsigned i = 0; i < itemName.GetCount(); i++)
	{
		itemsCreated[i] = new OpcItem(itemName[i],*this);
		USES_CONVERSION;
		WCHAR* wideName = T2OLE(itemName[i]);
		itemDef[i].szItemID = wideName;
		itemDef[i].szAccessPath = NULL;//wideName;
		itemDef[i].bActive = active;
		itemDef[i].hClient = (DWORD)itemsCreated[i];
		itemDef[i].dwBlobSize = 0;
		itemDef[i].pBlob = NULL;
		itemDef[i].vtRequestedDataType = VT_EMPTY;
	}

	HRESULT *itemResult;
	OPCITEMRESULT *itemDetails;
	DWORD noItems = (DWORD)itemName.GetCount();

	HRESULT	result = GetItemManagementInterface()->AddItems(noItems, itemDef, &itemDetails, &itemResult);
	delete[] itemDef;
	if (FAILED(result))
	{
		throw OpcException("Failed to add items");
	}

	int errorCount = 0;
	for (unsigned i = 0; i < noItems; i++)
	{
		if(itemDetails[i].pBlob)
		{ 
			OpcHost::ComFree(itemDetails[0].pBlob);
		}

		if (FAILED(itemResult[i]))
		{
			delete itemsCreated[i];
			itemsCreated[i] = NULL;
			errors[i] = itemResult[i];
			errorCount++;
		} 
		else 
		{
			(itemsCreated[i])->SetOpcParams(itemDetails[i].hServer, itemDetails[i].vtCanonicalDataType, itemDetails[i].dwAccessRights);
			errors[i] = ERROR_SUCCESS;
		}
	}

	OpcHost::ComFree(itemDetails);
	OpcHost::ComFree(itemResult);

	return errorCount;
}


void OpcGroup::EnableAsynch(IAsynchDataCallback &handler)
{
	if (!asynchDataCallBackHandler == false)
	{
		throw OpcException("Asynch already enabled");
	}

	CComPtr<IConnectionPointContainer> iConnectionPointContainer = 0;
	HRESULT result = iStateManagement->QueryInterface(IID_IConnectionPointContainer, (void**)&iConnectionPointContainer);
	if (FAILED(result))
	{
		throw OpcException("Could not get IID_IConnectionPointContainer");
	}


	result = iConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &iAsynchDataCallbackConnectionPoint);
	if (FAILED(result))
	{
		throw OpcException("Could not get IID_IOPCDataCallback");
	}


	asynchDataCallBackHandler = new AsyncDataCallback(*this);
	result = iAsynchDataCallbackConnectionPoint->Advise(asynchDataCallBackHandler, &callbackHandle);
	if (FAILED(result))
	{
		iAsynchDataCallbackConnectionPoint = NULL;
		asynchDataCallBackHandler = NULL;
		throw OpcException("Failed to set DataCallbackConnectionPoint");
	}

	userAsynchCBHandler = &handler;
}

void OpcGroup::SetState(DWORD reqUpdateRate_ms, DWORD &returnedUpdateRate_ms, float deadBand, BOOL active)
{
	HRESULT result = iStateManagement->SetState(&reqUpdateRate_ms, &returnedUpdateRate_ms, &active,0, &deadBand,0,0);
	if (FAILED(result))
	{
		throw OpcException("Failed to set group state");
	}
}

void OpcGroup::DisableAsynch()
{
	if (asynchDataCallBackHandler == NULL)
	{
		throw OpcException("Asynch is not exabled");
	}
	iAsynchDataCallbackConnectionPoint->Unadvise(callbackHandle);
	iAsynchDataCallbackConnectionPoint = NULL;
	asynchDataCallBackHandler = NULL;// DO NOT DELETE callbackHandler, let the COM ref counting take care of that
	userAsynchCBHandler = NULL;
}


OPCHANDLE* OpcGroup::BuildServerHandleList(CAtlArray<OpcItem *>& items)
{
	OPCHANDLE *handles = new OPCHANDLE[items.GetCount()];
	for (unsigned i = 0; i < items.GetCount(); i++)
	{
		if (items[i]==NULL)
		{
			delete []handles;
			throw OpcException("Item is NULL");
		}
		handles[i] = items[i]->GetHandle();
	}
	return handles;
}
