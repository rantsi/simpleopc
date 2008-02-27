#pragma once

// TrendGraphPropPage.h : Declaration of the CTrendGraphPropPage property page class.


// CTrendGraphPropPage : See TrendGraphPropPage.cpp for implementation.

class CTrendGraphPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CTrendGraphPropPage)
	DECLARE_OLECREATE_EX(CTrendGraphPropPage)

// Constructor
public:
	CTrendGraphPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_TRENDGRAPH };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

