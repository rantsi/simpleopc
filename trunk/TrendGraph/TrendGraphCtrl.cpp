// TrendGraphCtrl.cpp : Implementation of the CTrendGraphCtrl ActiveX Control class.

#include "stdafx.h"
#include "TrendGraph.h"
#include "TrendGraphCtrl.h"
#include "TrendGraphPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CTrendGraphCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CTrendGraphCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CTrendGraphCtrl, COleControl)
	DISP_FUNCTION_ID(CTrendGraphCtrl, "Append", dispidAppend, Append, VT_EMPTY, VTS_R8)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CTrendGraphCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CTrendGraphCtrl, 1)
	PROPPAGEID(CTrendGraphPropPage::guid)
END_PROPPAGEIDS(CTrendGraphCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTrendGraphCtrl, "TRENDGRAPH.TrendGraphCtrl.1",
	0x58714506, 0x5456, 0x42ef, 0x88, 0x49, 0xe2, 0x5f, 0x3e, 0x79, 0xc6, 0x51)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CTrendGraphCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DTrendGraph =
		{ 0xE8644C23, 0x411B, 0x4678, { 0xA2, 0xAB, 0x95, 0x11, 0x96, 0xAC, 0x28, 0xA5 } };
const IID BASED_CODE IID_DTrendGraphEvents =
		{ 0xBD179925, 0xE263, 0x4DAF, { 0xB6, 0x56, 0xDF, 0x96, 0xA, 0xFE, 0x9A, 0x54 } };



// Control type information

static const DWORD BASED_CODE _dwTrendGraphOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CTrendGraphCtrl, IDS_TRENDGRAPH, _dwTrendGraphOleMisc)



// CTrendGraphCtrl::CTrendGraphCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CTrendGraphCtrl

BOOL CTrendGraphCtrl::CTrendGraphCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_TRENDGRAPH,
			IDB_TRENDGRAPH,
			afxRegInsertable | afxRegApartmentThreading,
			_dwTrendGraphOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CTrendGraphCtrl::CTrendGraphCtrl - Constructor

CTrendGraphCtrl::CTrendGraphCtrl()
{
	InitializeIIDs(&IID_DTrendGraph, &IID_DTrendGraphEvents);
	
	m_minValue = 0;
	m_maxValue = 0;
}



// CTrendGraphCtrl::~CTrendGraphCtrl - Destructor

CTrendGraphCtrl::~CTrendGraphCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CTrendGraphCtrl::OnDraw - Drawing function

void CTrendGraphCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
//	pdc->Rect(rcBounds);
	//pdc->Ellipse(rcBounds);




	if (!IsOptimizedDraw())
	{
		// The container does not support optimized drawing.

		// TODO: if you selected any GDI objects into the device context *pdc,
		//		restore the previously-selected objects here.
	}
}



// CTrendGraphCtrl::DoPropExchange - Persistence support

void CTrendGraphCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CTrendGraphCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
DWORD CTrendGraphCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();


	// The control can activate without creating a window.
	// TODO: when writing the control's message handlers, avoid using
	//		the m_hWnd member variable without first checking that its
	//		value is non-NULL.
	dwFlags |= windowlessActivate;

	// The control can optimize its OnDraw method, by not restoring
	// the original GDI objects in the device context.
	dwFlags |= canOptimizeDraw;
	return dwFlags;
}



// CTrendGraphCtrl::OnResetState - Reset control to default state

void CTrendGraphCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CTrendGraphCtrl message handlers

void CTrendGraphCtrl::Append(DOUBLE newValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
}
