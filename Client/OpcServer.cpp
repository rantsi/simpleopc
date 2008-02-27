#include "StdAfx.h"
#include "OpcServer.h"
#include "OpcException.h"



OpcServer::OpcServer(CComPtr<IOPCServer> &opcServerInterface){
	iOpcServer = opcServerInterface;

	HRESULT res = opcServerInterface->QueryInterface(IID_IOPCBrowseServerAddressSpace, (void**)&iOpcNamespace);
	if (FAILED(res))
	{
		throw OpcException(CString("Failed to obtain IID_IOPCBrowseServerAddressSpace interface"),res);
	}

	res = opcServerInterface->QueryInterface(IID_IOPCItemProperties, (void**)&iOpcProperties);
	if (FAILED(res))
	{
		throw OpcException(CString("Failed to obtain IID_IOPCItemProperties interface"),res);
	}
}



OpcServer::~OpcServer()
{
}

void OpcServer::getItemNames(CAtlArray<CString> & opcItemNames)
{
	if (!iOpcNamespace) 
		return;

	OPCNAMESPACETYPE nameSpaceType;
	HRESULT result = iOpcNamespace->QueryOrganization(&nameSpaceType);

	USES_CONVERSION;
	int v = 0;
	WCHAR emptyString[] = {0};
	//result = iOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_TO,emptyString);

	CComPtr<IEnumString> iEnum;
	result = iOpcNamespace->BrowseOPCItemIDs(OPC_FLAT,emptyString,VT_EMPTY,0,(&iEnum));
	if (FAILED(result))
	{
		return;
	}


	 
    WCHAR * str;
    ULONG strSize;
	while((result = iEnum->Next(1, &str, &strSize)) == S_OK)
	{
		WCHAR * fullName;
		result = iOpcNamespace->GetItemID(str, &fullName);
		if (SUCCEEDED(result))
		{
			USES_CONVERSION;
			CString cStr(fullName);
			opcItemNames.Add(cStr);
			COPCClient::comFree(fullName);
		}
		COPCClient::comFree(str);
	}
}