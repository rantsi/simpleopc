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

protected:
	CComboBox m_CmbServers;
	CListCtrl m_LstTags;

private:
	OpcServer* m_OpcServer;
//	CAtlMap<CString*, DOUBLE> m_listItems;
	CAtlMap<CString*, DOUBLE> m_itemValues;
	CAtlMap<CString*,int> m_LstIndexes; 
	HANDLE m_tmrQueue;
	HANDLE m_timer;
	HANDLE m_mutex;
public:
	CNtgraphctrl3 m_graph3;
	CNtgraphctrl3 m_graph2;
	CNtgraphctrl3 m_graph1;
	CNtgraphctrl3 m_graph4;
};
