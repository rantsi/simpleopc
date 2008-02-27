#include "StdAfx.h"
#include "OpcHost.h"
#include "OpcException.h"


OpcHost::OpcHost(void)
{
}

OpcHost::~OpcHost(void)
{
}


void OpcHost::ListDaServers(CATID cid, CAtlArray<CString> &listOfProgIDs)
{
	CATID Implist[1];
	Implist[0] = cid;
	CComPtr<ICatInformation> iCatInfo;

	HRESULT result = CoCreateInstance (CLSID_StdComponentCategoriesMgr, NULL,CLSCTX_INPROC_SERVER, IID_ICatInformation,(void **)&iCatInfo);
	if (FAILED(result))
	{
		throw OpcException(CString("Failed to get IID_ICatInformation"));
	}

	ATL::CComPtr<IEnumCLSID> iEnum;
	result = iCatInfo->EnumClassesOfCategories(1, Implist,0, NULL,&iEnum);
	if (FAILED(result)){
		throw OpcException(CString("Failed to get enum for categeories"));
	}

	GUID glist;
	ULONG actual;
	while((result = iEnum->Next(1, &glist, &actual)) == S_OK)
	{
		WCHAR *progID;
		HRESULT res = ProgIDFromCLSID(glist, &progID);
		if(FAILED(res))
		{
			throw OpcException(CString("Failed to get ProgId from ClassId"));
		}
		else 
		{
			USES_CONVERSION;
			CString str(progID);
//			char * str = OLE2T(progID);
			listOfProgIDs.Add(str);
			COPCClient::comFree(progID);
		}
	}
}


OpcServer* OpcHost::ConnectDa(const CString & serverProgID){
	USES_CONVERSION;
	WCHAR* wideName = T2OLE(serverProgID);

	CLSID clsid;
	HRESULT result = CLSIDFromProgID(wideName, &clsid);
	if(FAILED(result))
	{
		throw OpcException(CString("Failed to convert progID to class ID"));
	}


	ATL::CComPtr<IClassFactory> iClassFactory;
	result = CoGetClassObject(clsid, CLSCTX_LOCAL_SERVER, NULL, IID_IClassFactory, (void**)&iClassFactory);
	if (FAILED(result))
	{
		throw OpcException(CString("Failed get Class factory"));
	}

	ATL::CComPtr<IUnknown> iUnknown;
	result = iClassFactory->CreateInstance(NULL, IID_IUnknown, (void**)&iUnknown);
	if (FAILED(result))
	{
		throw OpcException(CString("Failed get create OPC server ref"));
	}

	CComPtr<IOPCServer> iOpcServer;
	result = iUnknown->QueryInterface(IID_IOPCServer, (void**)&iOpcServer);
	if (FAILED(result))
	{
		throw OpcException(CString("Failed obtain IID_IOPCServer interface from server"));
	}

	return new OpcServer(iOpcServer);
}
