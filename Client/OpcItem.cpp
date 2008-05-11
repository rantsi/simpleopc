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

