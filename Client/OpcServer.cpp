#include "StdAfx.h"
#include "OpcServer.h"
#include "OpcException.h"
#include "OpcHost.h"
#include "OpcGroup.h"


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

void OpcServer::GetItemNames(CAtlArray<CString> & opcItemNames)
{
	if (!iOpcNamespace) 
		return;

	OPCNAMESPACETYPE nameSpaceType;
	HRESULT result = iOpcNamespace->QueryOrganization(&nameSpaceType);

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
			CString cStr(fullName);
			opcItemNames.Add(cStr);
			OpcHost::ComFree(fullName);
		}
		OpcHost::ComFree(str);
	}
}

OpcGroup* OpcServer::MakeGroup(const CString & groupName, bool active, unsigned long reqUpdateRate_ms, unsigned long &revisedUpdateRate_ms, float deadBand)
{
	return new OpcGroup(groupName, active, reqUpdateRate_ms, revisedUpdateRate_ms, deadBand, *this);
}
