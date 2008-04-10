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
	m_OpcServer(0), m_graphPoint(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_igName1 = "Graph.Graph Item 1";
	m_igName2 = "Graph.Graph Item 2";
	m_igName3 = "Graph.Graph Item 3";
	m_igName4 = "Graph.Graph Item 4";
	m_ilName1 = "List.List Item 1";
	m_ilName2 = "List.List Item 2";
	m_ilName3 = "List.List Item 3";
}

CClientDlg::~CClientDlg()
{
	// TODO: Release m_OpcServer
	DeleteCriticalSection(m_mutex);
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
	ON_WM_TIMER()
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

	ShowWindow(SW_NORMAL);

	// TODO: Add extra initialization here
	
	m_mutex = new CRITICAL_SECTION;
	InitializeCriticalSection(m_mutex);
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

	InitDisplay();

	OpcGroup* group = 0;
	OpcGroup* graphGroup = 0;

	try
	{
		CString server;
		m_CmbServers.GetLBText(m_CmbServers.GetCurSel(), server);

		m_OpcServer = OpcHost::ConnectDa(server);

		unsigned long refreshRate;
		group = m_OpcServer->MakeGroup("List", true, 500, refreshRate, 0.0);
		group->EnableAsynch(*this);

		CAtlArray<OpcItem*> listItems;
		listItems.Add(group->AddItem(m_ilName1, true));
		listItems.Add(group->AddItem(m_ilName2, true));
		listItems.Add(group->AddItem(m_ilName3, true));
		m_allItems.Append(listItems);
		group->ReadAsync(listItems, this);

		graphGroup = m_OpcServer->MakeGroup("Graph", true, 500, refreshRate, 0.0);
		graphGroup->EnableAsynch(*this);

		CAtlArray<OpcItem*> graphItems;
		graphItems.Add(graphGroup->AddItem(m_igName1, true));
		graphItems.Add(graphGroup->AddItem(m_igName2, true));
		graphItems.Add(graphGroup->AddItem(m_igName3, true));
		graphItems.Add(graphGroup->AddItem(m_igName4, true));
		m_allItems.Append(graphItems);
		graphGroup->ReadAsync(graphItems, this);

		SetTimer(1, 250, NULL);
	}
	catch (OpcException& oe)
	{
		MessageBox(oe.Reason(), "Error");

		if (group != 0)
			delete group;
		if (graphGroup != 0)
			delete graphGroup;
		if (m_OpcServer != 0)
			delete m_OpcServer;
	}
	catch (...)
	{
		MessageBox("Unknown error occurred.", "Error");

		if (group != 0)
			delete group;
		if (graphGroup != 0)
			delete graphGroup;
		if (m_OpcServer != 0)
			delete m_OpcServer;
	}
}

void CClientDlg::InitDisplay()
{
	m_LstTags.DeleteAllItems();
	m_LstTags.InsertColumn(0, "Tag", LVCFMT_LEFT, 180, 0);
	m_LstTags.InsertColumn(1, "Value", LVCFMT_LEFT, 100, 0);

	m_graph1.ClearGraph();
	m_graph1.SetElement(0);
	m_graph1.SetElementName(m_igName1);
	m_graph1.SetElementIdentify(TRUE);
	m_graph1.SetElementLineColor(RGB(255,0,0));
	m_graph1.SetElementWidth (1);
	m_graph1.SetElementSolidPoint(TRUE);

	m_graph2.ClearGraph();
	m_graph2.SetElement(0);
	m_graph2.SetElementName(m_igName2);
	m_graph2.SetElementIdentify(TRUE);
	m_graph2.SetElementLineColor(RGB(255,0,0));
	m_graph2.SetElementWidth (1);
	m_graph2.SetElementSolidPoint(TRUE);

	m_graph3.ClearGraph();
	m_graph3.SetElement(0);
	m_graph3.SetElementName(m_igName3);
	m_graph3.SetElementIdentify(TRUE);
	m_graph3.SetElementLineColor(RGB(255,0,0));
	m_graph3.SetElementWidth (1);
	m_graph3.SetElementSolidPoint(TRUE);

	m_graph4.ClearGraph();
	m_graph4.SetElement(0);
	m_graph4.SetElementName(m_igName4);
	m_graph4.SetElementIdentify(TRUE);
	m_graph4.SetElementLineColor(RGB(255,0,0));
	m_graph4.SetElementWidth (1);
	m_graph4.SetElementSolidPoint(TRUE);

	int i = 0;
	int idx = m_LstTags.InsertItem(i, m_ilName1);
	m_LstTags.SetItemText(idx, 1, "0");
	m_LstIndexes.SetAt(m_ilName1, idx);

	i++;
	idx = m_LstTags.InsertItem(i, m_ilName2);
	m_LstTags.SetItemText(idx, 1, "0");
	m_LstIndexes.SetAt(m_ilName2, idx);

	i++;
	idx = m_LstTags.InsertItem(i, m_ilName3);
	m_LstTags.SetItemText(idx, 1, "0");
	m_LstIndexes.SetAt(m_ilName3, idx);
}

void CClientDlg::Complete(Transaction &transaction)
{
	EnterCriticalSection(m_mutex);

	for(unsigned i = 0; i < m_allItems.GetCount(); i++)
	{
		CString name;
		name.Append(m_allItems.GetAt(i)->GetName());

		const OpcItemData* data = transaction.GetItemValue(m_allItems.GetAt(i));
		if (data != 0)
		{
			m_itemValues.SetAt(m_allItems.GetAt(i)->GetName(), data->vDataValue.dblVal);
		}
	}
	LeaveCriticalSection(m_mutex);
}

void CClientDlg::OnDataChange(OpcGroup& group, CAtlMap<OpcItem *, OpcItemData *> & changes)
{
	EnterCriticalSection(m_mutex);

	for(unsigned i = 0; i < m_allItems.GetCount(); i++)
	{
		CAtlMap<OpcItem*, OpcItemData*>::CPair* pos = changes.Lookup(m_allItems.GetAt(i));

		if (pos != NULL)
		{
			OpcItem* item = changes.GetKeyAt(pos);
			OpcItemData* data = changes.GetValueAt(pos);

			CString name;
			name.Append(item->GetName());
			CRBMap<CString, DOUBLE>::CPair* p = m_itemValues.Lookup(item->GetName());
			if (p != NULL)
			{
				p->m_value = data->vDataValue.dblVal;
			}
		}
	}
	LeaveCriticalSection(m_mutex);
}

void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_graphPoint++;

	Plot(m_graph1, m_igName1);
	Plot(m_graph2, m_igName2);
	Plot(m_graph3, m_igName3);
	Plot(m_graph4, m_igName4);

	UpdateListValue(m_ilName1);
	UpdateListValue(m_ilName2);
	UpdateListValue(m_ilName3);

	__super::OnTimer(nIDEvent);
}

void CClientDlg::Plot(CNtgraphctrl3& graph, CString& name)
{
	CRBMap<CString, DOUBLE>::CPair* p = m_itemValues.Lookup(name);
	if (p != NULL)
	{
		graph.PlotXY(m_graphPoint, p->m_value, 0);
		graph.AutoRange();
	}
}
void CClientDlg::UpdateListValue(CString& name)
{
	CRBMap<CString, int>::CPair* idxpos = m_LstIndexes.Lookup(name);
	CRBMap<CString, DOUBLE>::CPair* p = m_itemValues.Lookup(name);

	if (idxpos != NULL && p != NULL)
	{
		CString tmp;
		tmp.AppendFormat("%f", p->m_value);
		m_LstTags.SetItemText(idxpos->m_value, 1, tmp);
	}
}