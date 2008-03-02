#pragma once
#include "OpcHost.h"


struct  OpcItemData
{
	FILETIME ftTimeStamp;
	WORD wQuality;
	VARIANT vDataValue;
	HRESULT error;

	OpcItemData(HRESULT err);
	OpcItemData(FILETIME time, WORD qual, VARIANT & val, HRESULT err);
	OpcItemData();
	~OpcItemData();

	void Set(OPCITEMSTATE &itemState);

	void Set(FILETIME time, WORD qual, VARIANT & val);

	OpcItemData& operator=(OpcItemData &itemData);
};


// Forward
class OpcItem;

class OpcItem_DataMap:public CAtlMap<OpcItem *, OpcItemData *>
{
public:
	~OpcItem_DataMap();
};
