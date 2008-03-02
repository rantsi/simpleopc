#include "StdAfx.h"
#include "Transaction.h"
#include "OpcException.h"


Transaction::Transaction(ITransactionComplete * completeCB)
:completed(FALSE), cancelID(0xffffffff), completeCallBack(completeCB)
{
}



Transaction::Transaction(CAtlArray<OpcItem *>&items, ITransactionComplete * completeCB)
:completed(FALSE), cancelID(0xffffffff), completeCallBack(completeCB)
{
	for (unsigned i = 0; i < items.GetCount(); i++)
	{
		opcData.SetAt(items[i],NULL);
	}
}


void Transaction::SetItemError(OpcItem *item, HRESULT error)
{
	CAtlMap<OpcItem *, OpcItemData *>::CPair* pair = opcData.Lookup(item);
	opcData.SetValueAt(pair,new OpcItemData(error));
}



void Transaction::SetItemValue(OpcItem *item, FILETIME time, WORD qual, VARIANT & val, HRESULT err)
{
	CAtlMap<OpcItem *, OpcItemData *>::CPair* pair = opcData.Lookup(item);
	opcData.SetValueAt(pair,new OpcItemData(time, qual, val, err));
}


const OpcItemData* Transaction::GetItemValue(OpcItem *item) const
{
	const CAtlMap<OpcItem *, OpcItemData *>::CPair* pair = opcData.Lookup(item);
	if (!pair) 
	{
		throw OpcException("Value for item is not known.");
	}

	return pair->m_value;
}

void Transaction::SetCompleted()
{
	completed = TRUE;
	if (completeCallBack)
	{
		completeCallBack->Complete(*this);
	}
}

bool Transaction::IsCompeleted() const
{
	return completed;
}

void Transaction::SetCancelId(DWORD id)
{
	cancelID = id;
}

DWORD Transaction::GetCancelId() const
{
	return cancelID;
}

