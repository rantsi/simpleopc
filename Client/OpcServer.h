#pragma once
#include <atlbase.h>
#include <atlstr.h>
#include <atlexcept.h>
#include <atlcoll.h>
#include <objbase.h>
#include <COMCat.h>
#include "opcda.h"

class OpcGroup;

class OpcServer
{
public:
	OpcServer(CComPtr<IOPCServer> &opcServerInterface);

	/**
	* Browse the OPC servers namespace.
	* This is currently done FLAT mode
	*/
	void GetItemNames(CAtlArray<CString> & opcItemNames);

	OpcGroup* MakeGroup(const CString & groupName, bool active, unsigned long reqUpdateRate_ms, unsigned long &revisedUpdateRate_ms, float deadBand);

private:
	/**
	* IUnknown interface to the OPC server
	*/
	CComPtr<IOPCServer> iOpcServer;

	/**
	* Interface to the OPC server namespace
	*/
	CComPtr<IOPCBrowseServerAddressSpace> iOpcNamespace;

	/**
	* interface to the properties maintained for each item in the server namespace
	*/
	CComPtr<IOPCItemProperties> iOpcProperties;

	friend class OpcGroup;
	/**
	* Used by group object.
	*/
	CComPtr<IOPCServer> &GetServerInterface()
	{
		return iOpcServer;
	}

	friend class OpcItem;
	CComPtr<IOPCItemProperties> &GetPropertiesInterface()
	{
		return iOpcProperties;
	}

};
