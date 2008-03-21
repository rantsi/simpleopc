// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "OpcHost.h"
#include "OpcGroup.h"
#include "OpcException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CClientDlg dialog




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent),
	m_OpcServer(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CClientDlg::~CClientDlg()
{
	// TODO: Release m_OpcServer
}


void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CmbServers);
	DDX_Control(pDX, IDC_LIST1, m_LstTags);
	DDX_Control(pDX, IDC_NTGRAPHCTRL3, m_graph3);
	DDX_Control(pDX, IDC_NTGRAPHCTRL2, m_graph2);
	DDX_Control(pDX, IDC_NTGRAPHCTRL1, m_graph1);
	DDX_Control(pDX, IDC_NTGRAPHCTRL4, m_graph4);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO1, &CClientDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);

	// TODO: Add extra initialization here
	OpcHost::Init();

	CAtlArray<CString> localServerList;
	OpcHost::ListDaServers(IID_CATID_OPCDAServer20, localServerList);

	for (unsigned i = 0; i < localServerList.GetCount(); i++)
	{
		m_CmbServers.AddString(localServerList.GetAt(i));
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::OnCbnSelchangeCombo1()
{
	if (m_OpcServer != 0)
	{
		// TODO: Release m_OpcServer
	}

	m_LstTags.DeleteAllItems();

	CString server;
	m_CmbServers.GetLBText(m_CmbServers.GetCurSel(), server);

	m_OpcServer = OpcHost::ConnectDa(server);

	m_LstTags.DeleteAllItems();
	m_LstTags.InsertColumn(0, "Tag", LVCFMT_LEFT, 180, 0);
	m_LstTags.InsertColumn(1, "Value", LVCFMT_LEFT, 100, 0);

	// TODO: Set these and their values to be updat ed on timer...
	
	unsigned long refreshRate;
	OpcGroup* group = m_OpcServer->MakeGroup("ListGroup", true, 1000, refreshRate, 0.0);
	group->EnableAsynch(*this);

	CAtlArray<OpcItem*> m_listItems;
	m_listItems.Add(group->AddItem(CString("Bucket Brigade.Real8"), true));
	m_listItems.Add(group->AddItem(CString("Random.Real8"), true));
	group->ReadAsync(m_listItems, this);

	OpcGroup* graphGroup = m_OpcServer->MakeGroup("GraphGroup", true, 1000, refreshRate, 0.0);
	graphGroup->EnableAsynch(*this);

	CAtlArray<OpcItem*> m_graphItems;
	m_graphItems.Add(graphGroup->AddItem(CString("Bucket Brigade.Real8"), true));
	m_graphItems.Add(graphGroup->AddItem(CString("Random.Real8"), true));
	graphGroup->ReadAsync(m_graphItems, this);

}

void CClientDlg::Complete(Transaction &transaction)
{


	// Just items in list are here
	for(unsigned i = 0; i < m_listItems.GetCount(); i++)
	{
		OpcItem* item = m_listItems.GetAt(i);
		const OpcItemData* data = transaction.GetItemValue(item);

		int idx = m_LstTags.InsertItem(i, m_listItems.GetAt(i)->GetName());
		CString tmp;
		tmp.AppendFormat("%d", data->vDataValue.dblVal);
		m_LstTags.SetItemText(idx, 1, tmp);
		m_LstIndexes.SetAt(&m_listItems.GetAt(i)->GetName(), idx);
	}
}

void CClientDlg::OnDataChange(OpcGroup& group, CAtlMap<OpcItem *, OpcItemData *> & changes)
{
	if (group.GetName() == "ListGroup")
	{
		POSITION pos = changes.GetStartPosition();

		while (pos != NULL)
		{
			OpcItem* item = changes.GetKeyAt(pos);
			OpcItemData* data = changes.GetValueAt(pos);

			POSITION idxpos = m_LstIndexes.Lookup(&item->GetName());

			int idx = 0;
			if (idxpos != NULL)
			{
				idx = m_LstIndexes.GetValueAt(idxpos);
				CString tmp;
				tmp.AppendFormat("%d", data->vDataValue.dblVal);
				m_LstTags.SetItemText(idx, 1, tmp);
			}

			++pos;
		}
	}
	else if (group.GetName() == "GraphGroup")
	{
	}
}