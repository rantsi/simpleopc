#pragma once
#include "OpcHost.h"

// Holds desciption of a property for an OPC item.
struct PropertyDescription
{
	/// properties identifier
	DWORD id;

	/// server supplied textual description
	CString desc;

	/// data type of the property
	VARTYPE type;

	PropertyDescription(DWORD i, CString d, VARTYPE t):id(i),desc(d), type(t){};
};


// holds value for an OPC item property
struct PropertyValue
{
	/// the property that this value describes.
	const PropertyDescription & propDesc;

	/// properties value.
	VARIANT value;

	PropertyValue(const PropertyDescription &desc, VARIANT &val);

	~PropertyValue();
};
