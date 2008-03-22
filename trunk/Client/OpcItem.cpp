#include "StdAfx.h"
#include "OpcItem.h"
#include "OpcGroup.h"
#include "OpcException.h"

OpcItem::OpcItem(CString &itemName, OpcGroup &g):
name(itemName), group(g)
{
}

OpcItem::~OpcItem()
{
	HRESULT *itemResult;
	group.GetItemManagementInterface()->RemoveItems(1, &serversItemHandle, &itemResult);
	OpcHost::ComFree(itemResult);
}

DWORD OpcItem::GetAccessRights()
{
	return dwAccessRights;
}

OPCHANDLE OpcItem::GetHandle() 
{
	return serversItemHandle;
}	

CString& OpcItem::GetName() 
{
	return name;
} 

void OpcItem::SetOpcParams(OPCHANDLE handle, VARTYPE type, DWORD dwAccess)
{
	serversItemHandle	=handle; 
	vtCanonicalDataType	=type; 
	dwAccessRights		=dwAccess;
}

void OpcItem::WriteSync(VARIANT &data)
{
	HRESULT * itemWriteErrors; 
	HRESULT result = group.GetSychIOInterface()->Write(1, &serversItemHandle, &data, &itemWriteErrors);
	if (FAILED(result))
	{
		throw OpcException("write failed");
	} 

	if (FAILED(itemWriteErrors[0]))
	{
		OpcHost::ComFree(itemWriteErrors);
		throw OpcException("write failed");
	}

	OpcHost::ComFree(itemWriteErrors);
}

void OpcItem::ReadSync(OpcItemData &data, OPCDATASOURCE source)
{
	CAtlArray<OpcItem *> items;
	items.Add(this);
	OpcItem_DataMap opcData;
	group.ReadSync(items, opcData, source);
		
	OpcItem_DataMap::CPair* pos = opcData.Lookup(this);
	if (pos)
	{
		OpcItemData * readData = opcData.GetValueAt(pos);
		if (readData && !FAILED(readData->error))
		{
			data = *readData;
			return;
		}
	} 

	throw OpcException("Read failed");
}


Transaction* OpcItem::ReadAsynch(ITransactionComplete *transactionCB)
{
	CAtlArray<OpcItem *> items;
	items.Add(this);
	return group.ReadAsync(items, transactionCB);
}

Transaction* OpcItem::WriteAsynch(VARIANT &data, ITransactionComplete *transactionCB)
{
	DWORD cancelID;
	HRESULT * individualResults;
	CAtlArray<OpcItem *> items;
	items.Add(this);
	Transaction* trans = new Transaction(items,transactionCB);

	HRESULT result = group.GetAsych2IOInterface()->Write(1,&serversItemHandle,&data,(DWORD)trans,&cancelID,&individualResults); 
	
	if (FAILED(result))
	{
		delete trans;
		throw OpcException("Asynch Write failed");
	}

	trans->SetCancelId(cancelID);
	if (FAILED(individualResults[0]))
	{
		trans->SetItemError(this,individualResults[0]);
		trans->SetCompleted(); // if all items return error then no callback will occur.
	}

	OpcHost::ComFree(individualResults);
	return trans;
}

void OpcItem::GetSupportedProperties(CAtlArray<PropertyDescription> &desc){
	DWORD noProperties = 0;
	DWORD *pPropertyIDs;
	LPWSTR *pDescriptions;
	VARTYPE *pvtDataTypes;

	USES_CONVERSION;
	HRESULT res = group.GetServer().GetPropertiesInterface()->QueryAvailableProperties(T2OLE(name), &noProperties, &pPropertyIDs, &pDescriptions, &pvtDataTypes);
	if (FAILED(res))
	{
		throw OpcException("Failed to restrieve properties", res);
	}

	for (unsigned i = 0; i < noProperties; i++)
	{
		desc.Add(PropertyDescription(pPropertyIDs[i], CString(pDescriptions[i]), pvtDataTypes[i]));
	}

	OpcHost::ComFree(pPropertyIDs);
	OpcHost::ComFree(pDescriptions);
	OpcHost::ComFree(pvtDataTypes);
}


void OpcItem::GetProperties(const CAtlArray<PropertyDescription> &propsToRead, ATL::CAutoPtrArray<PropertyValue> &propsRead){
	unsigned noProperties = (DWORD)propsToRead.GetCount();
	VARIANT *pValues = NULL;
	HRESULT *pErrors = NULL;
	DWORD *pPropertyIDs = new DWORD[noProperties];
	for (unsigned i = 0; i < noProperties; i++)
	{
		pPropertyIDs[i] = propsToRead.GetAt(i).id;
	}
	propsRead.RemoveAll();
	propsRead.SetCount(noProperties);
	
	USES_CONVERSION;
	HRESULT res = group.GetServer().GetPropertiesInterface()->GetItemProperties(T2OLE(name), noProperties, pPropertyIDs, &pValues, &pErrors);
	delete []pPropertyIDs;
	if (FAILED(res))
	{
		throw OpcException("Failed to retrieve property values", res);
	}

	for (unsigned i = 0; i < noProperties; i++)
	{
		CAutoPtr<PropertyValue> v;
		if (!FAILED(pErrors[i]))
		{
			v.Attach(new PropertyValue(propsToRead[i], pValues[i]));
		}
		propsRead[i]=v;
	}

	OpcHost::ComFree(pErrors);
	OpcHost::ComFreeVariant(pValues, noProperties);
}
