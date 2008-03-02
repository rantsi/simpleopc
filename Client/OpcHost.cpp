#include "StdAfx.h"
#include "OpcHost.h"
#include "OpcException.h"

CComPtr<IMalloc> OpcHost::iMalloc;


OpcHost::OpcHost(void)
{
}

OpcHost::~OpcHost(void)
{
}

void OpcHost::Init()
{	
	HRESULT	result = CoInitialize(NULL);
	if (FAILED(result))
	{
		throw OpcException("CoInitialize failed");
	}

	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

	result = CoGetMalloc(MEMCTX_TASK, &iMalloc);
	if (FAILED(result))
	{
		throw OpcException("CoGetMalloc failed");
	}
}

void OpcHost::Stop()
{
	iMalloc = NULL;
	CoUninitialize();
}

void OpcHost::ComFree(void *memory)
{
	iMalloc->Free(memory);
}


void OpcHost::ComFreeVariant(VARIANT *memory, unsigned size){
	for (unsigned i = 0; i < size; i++)
	{
		VariantClear(&(memory[i]));	
	}
	iMalloc->Free(memory);
}

void OpcHost::ListDaServers(CATID cid, CAtlArray<CString> &listOfProgIDs)
{
	CATID Implist[1];
	Implist[0] = cid;
	CComPtr<ICatInformation> iCatInfo;

	HRESULT result = CoCreateInstance (CLSID_StdComponentCategoriesMgr, NULL,CLSCTX_INPROC_SERVER, IID_ICatInformation,(void **)&iCatInfo);
	if (FAILED(result))
	{
		throw OpcException("Failed to get IID_ICatInformation");
	}

	CComPtr<IEnumCLSID> iEnum;
	result = iCatInfo->EnumClassesOfCategories(1, Implist,0, NULL,&iEnum);
	if (FAILED(result))
	{
		throw OpcException("Failed to get enum for categeories");
	}

	GUID glist;
	ULONG actual;
	while((result = iEnum->Next(1, &glist, &actual)) == S_OK)
	{
		WCHAR *progID;
		HRESULT res = ProgIDFromCLSID(glist, &progID);
		if(FAILED(res))
		{
			throw OpcException("Failed to get ProgId from ClassId");
		}
		else 
		{
			USES_CONVERSION;
			CString str(progID);
			listOfProgIDs.Add(str);
			OpcHost::ComFree(progID);
		}
	}
}


OpcServer* OpcHost::ConnectDa(const CString &serverProgID){
	USES_CONVERSION;
	WCHAR* wideName = T2OLE(serverProgID);

	CLSID clsid;
	HRESULT result = CLSIDFromProgID(wideName, &clsid);
	if(FAILED(result))
	{
		throw OpcException("Failed to convert progID to class ID");
	}


	CComPtr<IClassFactory> iClassFactory;
	result = CoGetClassObject(clsid, CLSCTX_LOCAL_SERVER, NULL, IID_IClassFactory, (void**)&iClassFactory);
	if (FAILED(result))
	{
		throw OpcException("Failed get Class factory");
	}

	ATL::CComPtr<IUnknown> iUnknown;
	result = iClassFactory->CreateInstance(NULL, IID_IUnknown, (void**)&iUnknown);
	if (FAILED(result))
	{
		throw OpcException("Failed get create OPC server ref");
	}

	CComPtr<IOPCServer> iOpcServer;
	result = iUnknown->QueryInterface(IID_IOPCServer, (void**)&iOpcServer);
	if (FAILED(result))
	{
		throw OpcException("Failed obtain IID_IOPCServer interface from server");
	}

	return new OpcServer(iOpcServer);
}
