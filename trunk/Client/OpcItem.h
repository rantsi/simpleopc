#pragma once
#include "OpcHost.h"
#include "Transaction.h"
#include "OpcItemData.h"
#include "Property.h"

// Forward declaration
class OpcGroup;

class OpcItem
{
public:
	~OpcItem();

	void WriteSync(VARIANT &data);


	void ReadSync(OpcItemData &data, OPCDATASOURCE source);

	/**
	* returned transaction object is owned
	*/
	Transaction* ReadAsynch(ITransactionComplete *transactionCB = NULL);


	/**
	* returned transaction object is owned
	*/
	Transaction * WriteAsynch(VARIANT &data, ITransactionComplete *transactionCB = NULL);

	DWORD GetAccessRights();
	OPCHANDLE GetHandle();
	CString & GetName();

	void GetSupportedProperties(CAtlArray<PropertyDescription> &desc);

	
	/**
	* retreive the OPC item properties for the descriptors passed. Any data previously existing in propsRead will be destroyed.
	*/
	void GetProperties(const CAtlArray<PropertyDescription> &propsToRead, ATL::CAutoPtrArray<PropertyValue> &propsRead);

private:
	OPCHANDLE serversItemHandle;
    VARTYPE vtCanonicalDataType;
    DWORD dwAccessRights;

	OpcGroup & group;

	CString name;
	
protected:
	friend class OpcGroup;
	// used to set data for the OPC item AFTER it has been created in the server.
	void SetOpcParams(OPCHANDLE handle, VARTYPE type, DWORD dwAccess);

	// items may only be created by group.
	OpcItem(CString &itemName, OpcGroup &g);
};
