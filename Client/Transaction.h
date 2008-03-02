#pragma once
#include "OpcHost.h"
#include "OpcItemData.h"


// Forward
class Transaction;
class OpcItem;

class ITransactionComplete
{
public:
	virtual void Complete(Transaction &transaction) = 0;
};


class Transaction
{
public:
	/**
	* keyed on OPCitem address (not owned)
	* OpcItem data is owned by the transaction - may be NULL
	*/
	OpcItem_DataMap opcData;

	Transaction(ITransactionComplete * completeCB = NULL);

	/**
	* Used where the transaction completion will result in data being received.
	*/
	Transaction(CAtlArray<OpcItem *>&items, ITransactionComplete * completeCB);

	void SetItemError(OpcItem *item, HRESULT error);

	void SetItemValue(OpcItem *item, FILETIME time, WORD qual, VARIANT & val, HRESULT err);

	// return Value stored for a given opc item.
	const OpcItemData* GetItemValue(OpcItem *item) const;
			
	// trigger completion of the transaction.
	void SetCompleted();

	bool IsCompeleted() const;

	void SetCancelId(DWORD id);

	DWORD GetCancelId() const;
private:
	// Optional transation complete callback - not owned
	ITransactionComplete * completeCallBack;

	// true when the transaction has completed
	bool completed;

	DWORD cancelID;
};
