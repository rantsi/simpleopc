#pragma once
#include <atlbase.h>
#include <atlstr.h>
#include <atlexcept.h>
#include <objbase.h>
#include <stdexcept>

class OpcException
{
public:
	OpcException(const CString& what, HRESULT code = 0): why(what){}

	const CString & Reason() const
	{
		return why;
	}

private:
	CString why;

};
