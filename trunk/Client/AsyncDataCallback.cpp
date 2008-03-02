#include "StdAfx.h"
#include "AsyncDataCallback.h"
#include "OpcGroup.h"

AsyncDataCallback::AsyncDataCallback(OpcGroup &group):callbacksGroup(group)
{
	mRefCount = 0;
}

AsyncDataCallback::~AsyncDataCallback(void)
{
}


STDMETHODIMP AsyncDataCallback::QueryInterface( REFIID iid, LPVOID* ppInterface){
	if ( ppInterface == NULL)
	{
		return E_INVALIDARG;
	}

	if ( iid == IID_IUnknown )
	{
		*ppInterface = (IUnknown*) this;
	} 
	else if ( iid == IID_IOPCDataCallback)
	{
		*ppInterface = (IOPCDataCallback*) this;
	} 
	else
	{
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) AsyncDataCallback::AddRef()
{
	return ++mRefCount;
}


STDMETHODIMP_(ULONG) AsyncDataCallback::Release()
{
	--mRefCount; 

	if ( mRefCount == 0)
	{
		delete this;
	}
	return mRefCount;
}

STDMETHODIMP AsyncDataCallback::OnDataChange(DWORD Transid, OPCHANDLE grphandle, HRESULT masterquality,
	HRESULT mastererror, DWORD count, OPCHANDLE * clienthandles, 
	VARIANT * values, WORD * quality, FILETIME  * time,
	HRESULT * errors)
{
	IAsynchDataCallback * usrHandler = callbacksGroup.GetUsrAsynchHandler();

	if (Transid != 0)
	{
		// it is a result of a refresh (see p106 of spec)
		Transaction & trans = *(Transaction *)Transid; 
		UpdateOpcData(trans.opcData, count, clienthandles, values,quality,time,errors);
		trans.SetCompleted();	
		return S_OK;	
	}

	if (usrHandler)
	{
		OpcItem_DataMap dataChanges;
		UpdateOpcData(dataChanges, count, clienthandles, values,quality,time,errors);
		usrHandler->OnDataChange(callbacksGroup, dataChanges);
	}
	return S_OK;
}


STDMETHODIMP AsyncDataCallback::OnReadComplete(DWORD Transid, OPCHANDLE grphandle, 
	HRESULT masterquality, HRESULT mastererror, DWORD count, 
	OPCHANDLE * clienthandles, VARIANT* values, WORD * quality,
	FILETIME * time, HRESULT * errors)
{
	// TODO this is bad  - server could corrupt address - need to use look up table
	Transaction & trans = *(Transaction *)Transid; 
	UpdateOpcData(trans.opcData, count, clienthandles, values,quality,time,errors);
	trans.SetCompleted();
	return S_OK;
}


STDMETHODIMP AsyncDataCallback::OnWriteComplete(DWORD Transid, OPCHANDLE grphandle, HRESULT mastererr, 
	DWORD count, OPCHANDLE * clienthandles, HRESULT * errors)
{
	// TODO this is bad  - server could corrupt address - need to use look up table
	Transaction & trans = *(Transaction *)Transid; 

	// see page 145 - number of items returned may be less than sent
	for (unsigned i = 0; i < count; i++)
	{
		// TODO this is bad  - server could corrupt address - need to use look up table
		OpcItem * item = (OpcItem *)clienthandles[i];
		trans.SetItemError(item, errors[i]); // this records error state - may be good
	}

	trans.SetCompleted();
	return S_OK;
}



STDMETHODIMP AsyncDataCallback::OnCancelComplete(DWORD transid, OPCHANDLE grphandle){
	printf("OnCancelComplete: Transid=%ld GrpHandle=%ld\n", transid, grphandle);
	return S_OK;
}


/**
* make OPC item
*/
OpcItemData * AsyncDataCallback::MakeOpcDataItem(VARIANT& value, WORD quality, FILETIME & time, HRESULT error)
{
	OpcItemData * data = NULL;
	if (FAILED(error))
	{
		data = new OpcItemData(error);
	} 
	else 
	{	
		data = new OpcItemData(time,quality,value,error);	
	}
	return data;
}

/**
* Enter the OPC items data that resulted from an operation
*/
void AsyncDataCallback::UpdateOpcData(OpcItem_DataMap &opcData, DWORD count, OPCHANDLE * clienthandles, 
	VARIANT* values, WORD * quality,FILETIME * time, HRESULT * errors)
{
	// see page 136 - returned arrays may be out of order
	for (unsigned i = 0; i < count; i++)
	{
		// TODO this is bad  - server could corrupt address - need to use look up table
		OpcItem * item = (OpcItem *)clienthandles[i];
		OpcItemData * data = MakeOpcDataItem(values[i], quality[i], time[i], errors[i]);
		OpcItem_DataMap::CPair* pair = opcData.Lookup(item);
		if (pair == NULL)
		{
			opcData.SetAt(item,data);
		} 
		else 
		{
			opcData.SetValueAt(pair,data);
		}
	}
}

