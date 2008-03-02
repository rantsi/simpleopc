// ClientDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "OpcServer.h"
#include "afxcmn.h"

// CClientDlg dialog
class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor
	~CClientDlg();

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


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
protected:
	CComboBox m_CmbServers;
	CListCtrl m_LstTags;

private:
	OpcServer* m_OpcServer;
};
