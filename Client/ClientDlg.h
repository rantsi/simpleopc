// ClientDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "OpcServer.h"
#include "OpcItem.h"
#include "OpcGroup.h"
#include "afxcmn.h"
#include "ntgraphctrl3.h"

// CClientDlg dialog
class CClientDlg : public CDialog, ITransactionComplete, IAsynchDataCallback
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor
	~CClientDlg();

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void OnDataChange(OpcGroup& group, CAtlMap<OpcItem *, OpcItemData *> & changes);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	void Complete(Transaction &transaction);
	void UpdateDisplay();

protected:
	void InitDisplay();
	void Plot(CNtgraphctrl3& graph, CString& name);
	void UpdateListValue(CString& name);

	CComboBox m_CmbServers;
	CListCtrl m_LstTags;

private:
	OpcServer* m_OpcServer;
	CAtlArray<OpcItem*> m_allItems;
	CRBMap<CString, DOUBLE> m_itemValues;
	CRBMap<CString, int> m_LstIndexes; 
	double m_graphPoint;
	LPCRITICAL_SECTION m_mutex;

	CString m_igName1;
	CString m_igName2;
	CString m_igName3;
	CString m_igName4;
	CString m_ilName1;
	CString m_ilName2;
	CString m_ilName3;
public:
	CNtgraphctrl3 m_graph3;
	CNtgraphctrl3 m_graph2;
	CNtgraphctrl3 m_graph1;
	CNtgraphctrl3 m_graph4;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
