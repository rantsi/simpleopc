#pragma once
#include <atlbase.h>
#include <atlstr.h>
#include <atlexcept.h>
#include <atlcoll.h>
#include <objbase.h>
#include <COMCat.h>
#include "opcda.h"

class OpcServer
{
public:
	OpcServer(CComPtr<IOPCServer> &opcServerInterface);
	~OpcServer(void);
};
