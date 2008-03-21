#include "StdAfx.h"
#include "Property.h"
#include "OpcException.h"


PropertyValue::PropertyValue(const PropertyDescription &desc, VARIANT &val):propDesc(desc)
{
	value.vt = VT_EMPTY;
	HRESULT result = VariantCopy( &value, &val);
	if (FAILED(result))
	{
		throw OpcException("VariantCopy failed", result);
	}
}

PropertyValue::~PropertyValue()
{
	VariantClear(&value);
}
