#include "StdAfx.h"
#include "OpcItemData.h"
#include "OpcException.h"


OpcItemData::OpcItemData(HRESULT err):error(err)
{
	vDataValue.vt = VT_EMPTY;
}

OpcItemData::OpcItemData(FILETIME time, WORD qual, VARIANT & val, HRESULT err)
{
	vDataValue.vt = VT_EMPTY;
	HRESULT result = VariantCopy( &vDataValue, &val);
	if (FAILED(result))
	{
		throw OpcException("VariantCopy failed");
	}

	ftTimeStamp = time;
	wQuality = qual;
	error = err;
}

OpcItemData::OpcItemData()
{
	vDataValue.vt = VT_EMPTY;
}

OpcItemData::~OpcItemData()
{
	VariantClear(&vDataValue);
}

void OpcItemData::Set(OPCITEMSTATE &itemState)
{
	HRESULT result = VariantCopy( &vDataValue, &itemState.vDataValue);
	if (FAILED(result))
	{
		throw OpcException("VariantCopy failed");
	}

	ftTimeStamp = itemState.ftTimeStamp;
	wQuality = itemState.wQuality;
}


void OpcItemData::Set(FILETIME time, WORD qual, VARIANT & val)
{
	HRESULT result = VariantCopy( &vDataValue, &val);
	if (FAILED(result))
	{
		throw OpcException("VariantCopy failed");
	}

	ftTimeStamp = time;
	wQuality = qual;
}

OpcItemData & OpcItemData::operator=(OpcItemData &itemData)
{
	HRESULT result = VariantCopy( &vDataValue, &(itemData.vDataValue));
	if (FAILED(result))
	{
		throw OpcException("VariantCopy failed");
	}

	ftTimeStamp = itemData.ftTimeStamp;
	wQuality = itemData.wQuality;

	return *this;
}


OpcItem_DataMap::~OpcItem_DataMap(){
	POSITION pos = GetStartPosition();
	while (pos != NULL)
	{
		OpcItemData * data = GetNextValue(pos);
		if (data)
		{
			delete data;
		}
	}
	RemoveAll();
}
