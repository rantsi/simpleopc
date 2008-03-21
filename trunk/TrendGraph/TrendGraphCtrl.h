#pragma once

// TrendGraphCtrl.h : Declaration of the CTrendGraphCtrl ActiveX Control class.


// CTrendGraphCtrl : See TrendGraphCtrl.cpp for implementation.

class CTrendGraphCtrl : public COleControl
{
	DECLARE_DYNCREATE(CTrendGraphCtrl)

// Constructor
public:
	CTrendGraphCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();

// Implementation
protected:
	~CTrendGraphCtrl();

	DECLARE_OLECREATE_EX(CTrendGraphCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CTrendGraphCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CTrendGraphCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CTrendGraphCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		dispidAppend = 1L
	};
protected:
	void Append(DOUBLE newValue);

private:
	DOUBLE m_minValue;
	DOUBLE m_maxValue;
};

