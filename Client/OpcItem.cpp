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
