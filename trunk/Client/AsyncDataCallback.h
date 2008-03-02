#pragma once
#include "OpcHost.h"

// Forward
class OpcGroup;

class AsyncDataCallback : public IOPCDataCallback
{
public:
	AsyncDataCallback(OpcGroup &group);
	~AsyncDataCallback();

	STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	STDMETHODIMP OnDataChange(DWORD Transid, OPCHANDLE grphandle, HRESULT masterquality,
		HRESULT mastererror, DWORD count, OPCHANDLE * clienthandles, 
		VARIANT * values, WORD * quality, FILETIME  * time,
		HRESULT * errors);
	STDMETHODIMP OnReadComplete(DWORD Transid, OPCHANDLE grphandle, 
		HRESULT masterquality, HRESULT mastererror, DWORD count, 
		OPCHANDLE * clienthandles, VARIANT* values, WORD * quality,
		FILETIME * time, HRESULT * errors);
	STDMETHODIMP OnWriteComplete(DWORD Transid, OPCHANDLE grphandle, HRESULT mastererr, 
		DWORD count, OPCHANDLE * clienthandles, HRESULT * errors);
	STDMETHODIMP OnCancelComplete(DWORD transid, OPCHANDLE grphandle);

	static OpcItemData * MakeOpcDataItem(VARIANT& value, WORD quality, FILETIME & time, HRESULT error);
	static void UpdateOpcData(OpcItem_DataMap &opcData, DWORD count, OPCHANDLE * clienthandles, 
		VARIANT* values, WORD * quality,FILETIME * time, HRESULT * errors);

private:
	DWORD mRefCount;

	/**
	* group this is a callback for
	*/
	OpcGroup &callbacksGroup;


};
