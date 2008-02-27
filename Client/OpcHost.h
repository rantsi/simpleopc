#pragma once
#include <atlbase.h>
#include <atlstr.h>
#include <atlexcept.h>
#include <atlcoll.h>
#include <objbase.h>
#include <COMCat.h>
#include "OpcServer.h"

class OpcHost
{
public:
	OpcHost(void);
	~OpcHost(void);

	void ListDaServers(CATID cid, CAtlArray<CString> &listOfProgIDs);

	OpcServer* ConnectDa(const CString &serverProgID);
};
