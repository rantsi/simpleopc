#pragma once
#include <atlbase.h>
#include <atlconv.h>
#include <tchar.h> 
#include <atlstr.h>
#include <atlexcept.h>
#include <atlcoll.h>
#include <objbase.h>
#include <COMCat.h>
#include <stdexcept>
#include "opcda.h"
#include "OpcServer.h"
#include "OpcItemData.h"



class OpcHost
{
public:
	OpcHost(void);
	~OpcHost(void);

	static void Init();
	static void Stop();
	static void ComFree(void *memory);
	static void ComFreeVariant(VARIANT *memory, unsigned size);

	static void ListDaServers(CATID cid, CAtlArray<CString> &listOfProgIDs);
	static OpcServer* ConnectDa(const CString &serverProgID);

private:
	static CComPtr<IMalloc> iMalloc; 
};

class OpcItem;
class OpcGroup;
struct OpcItemData;


class IAsynchDataCallback
{
public:
	virtual void OnDataChange(OpcGroup & group, CAtlMap<OpcItem*, OpcItemData*> & changes) = 0;
};
