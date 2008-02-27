// TrendGraph.cpp : Implementation of CTrendGraphApp and DLL registration.

#include "stdafx.h"
#include "TrendGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTrendGraphApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x2911BACA, 0x4701, 0x4A9C, { 0xB3, 0x1E, 0xEB, 0x9C, 0x7B, 0xA5, 0x81, 0x88 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CTrendGraphApp::InitInstance - DLL initialization

BOOL CTrendGraphApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CTrendGraphApp::ExitInstance - DLL termination

int CTrendGraphApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
