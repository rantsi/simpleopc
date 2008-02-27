// TrendGraphPropPage.cpp : Implementation of the CTrendGraphPropPage property page class.

#include "stdafx.h"
#include "TrendGraph.h"
#include "TrendGraphPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CTrendGraphPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CTrendGraphPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTrendGraphPropPage, "TRENDGRAPH.TrendGraphPropPage.1",
	0xfa3cc0a9, 0x42e9, 0x49ca, 0xa0, 0xad, 0x74, 0xc6, 0x5e, 0x54, 0x66, 0x48)



// CTrendGraphPropPage::CTrendGraphPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CTrendGraphPropPage

BOOL CTrendGraphPropPage::CTrendGraphPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_TRENDGRAPH_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CTrendGraphPropPage::CTrendGraphPropPage - Constructor

CTrendGraphPropPage::CTrendGraphPropPage() :
	COlePropertyPage(IDD, IDS_TRENDGRAPH_PPG_CAPTION)
{
}



// CTrendGraphPropPage::DoDataExchange - Moves data between page and properties

void CTrendGraphPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CTrendGraphPropPage message handlers
