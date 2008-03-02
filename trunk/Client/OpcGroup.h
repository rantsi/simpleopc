#pragma once
#include "OpcHost.h"
#include "Transaction.h"
#include "AsyncDataCallback.h"

// Forward declaration
class OpcItem;

class OpcGroup
{
public:
	OpcGroup(const CString & groupName, bool active, unsigned long reqUpdateRate_ms, unsigned long &revisedUpdateRate_ms, float deadBand, OpcServer &server);

	virtual ~OpcGroup();


	OpcItem * AddItem(CString &itemName, bool active);

	/**
	* returns the number of failed item creates
	* itemsCreated[x] will be null if could not create and will contain error code in corresponding error entry
	*/
	int AddItems(CAtlArray<CString>& itemName, CAtlArray<OpcItem *>& itemsCreated, CAtlArray<HRESULT>& errors, bool active);


	/**
	* enable Asynch IO
	*/
	void EnableAsynch(IAsynchDataCallback &handler);


	/**
	* disable Asych IO 
	*/
	void DisableAsynch();


	/**
	* set the group state values.
	*/
	void SetState(DWORD reqUpdateRate_ms, DWORD &returnedUpdateRate_ms, float deadBand, BOOL active);

	/**
	* Read set of OPC items synchronously.
	*/
	void ReadSync(CAtlArray<OpcItem *>& items, OpcItem_DataMap &opcData, OPCDATASOURCE source);

	/**
	* Read a defined group of OPC item asynchronously
	*/
	Transaction * ReadAsync(CAtlArray<OpcItem *>& items, ITransactionComplete *transactionCB = NULL);


	/**
	* Refresh is an asysnch operation.
	* retreives all active items in the group, which will be stored in the transaction object
	* Transaction object is owned by caller.
	* If group asynch is disabled then this call will not work
	*/ 
	Transaction * Refresh(OPCDATASOURCE source, ITransactionComplete *transactionCB = NULL);
	CComPtr<IOPCSyncIO> & GetSychIOInterface();
	CComPtr<IOPCAsyncIO2>& GetAsych2IOInterface();
	CComPtr<IOPCItemMgt>& GetItemManagementInterface();
	const CString& GetName(); 
	IAsynchDataCallback *GetUsrAsynchHandler();

	/**
	* returns reaference to the OPC server that this group belongs to.
	*/
	OpcServer & GetServer();

private:
	CComPtr<IOPCGroupStateMgt>	iStateManagement;
	CComPtr<IOPCSyncIO>		iSychIO;
	CComPtr<IOPCAsyncIO2>		iAsych2IO;
	CComPtr<IOPCItemMgt>		iItemManagement;

	/**
	* Used to keep track of the connection point for the
	* AsynchDataCallback
	*/
	CComPtr<IConnectionPoint> iAsynchDataCallbackConnectionPoint;


	/**
	* handle given the group by the server
	*/
	DWORD groupHandle;

	/**
	* The server this group belongs to
	*/
	OpcServer &opcServer;


	/**
	* Callback for asynch data at the group level
	*/
	CComPtr<AsyncDataCallback> asynchDataCallBackHandler;


	/**
	* list of OPC items associated with this goup. Not owned (at the moment!)
	*/
	CAtlArray<OpcItem *> items;

	/**
	* Name of the group
	*/
	const CString name;


	/**
	* Handle given to callback by server.
	*/
	DWORD callbackHandle;


	/**
	* Users hander to handle asynch data 
	* NOT OWNED.
	*/
	IAsynchDataCallback *userAsynchCBHandler;
	AsyncDataCallback* _AsynchDataCallback;

	/**
	* Caller owns returned array
	*/
	OPCHANDLE * BuildServerHandleList(CAtlArray<OpcItem *>& items);
};



