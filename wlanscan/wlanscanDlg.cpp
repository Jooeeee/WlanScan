// wlanscanDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "wlanscan.h"
#include "wlanscanDlg.h"
#include "DevDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"
UINT threadFunc ( LPVOID p );

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
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


// CwlanscanDlg-Dialogfeld




CwlanscanDlg::CwlanscanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CwlanscanDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwlanscanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
	DDX_Control(pDX, IDC_STATICTEXT, LabelInfo);
}

BEGIN_MESSAGE_MAP(CwlanscanDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CwlanscanDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CwlanscanDlg-Meldungshandler

BOOL CwlanscanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
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

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	DWORD dwStyle = m_ctlList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_ctlList.SetExtendedStyle(dwStyle);



	this->m_ctlList.InsertColumn(0,"Ssid",LVCFMT_LEFT,75);
	this->m_ctlList.InsertColumn(1,"BSSID",LVCFMT_LEFT,75);
	this->m_ctlList.InsertColumn(2,"Signal",LVCFMT_LEFT,75);
	this->m_ctlList.InsertColumn(3,"Encryption",LVCFMT_LEFT,75);
	this->m_ctlList.InsertColumn(4,"Cannel",LVCFMT_LEFT,75);
	this->m_ctlList.InsertColumn(5,"BeaconPeriod",LVCFMT_LEFT,75);

	//such dir erst ma so ne WLANKARTE
	DevDialog devd(this);

	m_airctl.list_devices(); //list them intern

	devd.DoModal();
	
	deviceInfo* dv = m_airctl.getDevList();//get selceted divicenumber

	if (dv != NULL)//is not empty
	{
		//step to selceted dev
		for (int i = 0 ; i<devd.LastSel;i++) 
		{
			dv = dv->next;
		}

		//try to open and start scanthread
		if(m_airctl.open(dv->name)!= true)
		{
			MessageBox("Cant open selected device","fehler",0);
		}else
		{
			::AfxBeginThread( threadFunc , (LPVOID) this);

		}
	}
	else
	{
		MessageBox("no available device","error",0);
	}

		return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CwlanscanDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CwlanscanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CwlanscanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**
*thraed that scans for wlan and fills the list.
*/
UINT threadFunc ( LPVOID p )
{
	CwlanscanDlg* pDlg=(CwlanscanDlg*) p;

	MSG   msg ;
	
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE)	; // Force to make the queue
// 	pDlg->m_threadID = GetCurrentThreadId() ;

	while( TRUE )
	{
        if( PeekMessage( &msg , 0 , WM_CLOSE,WM_CLOSE,PM_NOREMOVE ) )
		{
 
			break ;
		}
		pDlg->LabelInfo.SetWindowTextA("scanning.");
		//get list
		NDIS_802_11_BSSID_LIST * pBSSIDList = pDlg->m_airctl.scan();
		pDlg->LabelInfo.SetWindowTextA("scanning..");

		if (pBSSIDList != NULL)
		{
			LVITEM lvi;
			CString strItem;

			pDlg->m_ctlList.DeleteAllItems();
			lvi.iItem =pDlg->m_ctlList.GetItemCount() ;

		// Insert the first item
			lvi.mask = LVIF_TEXT;
		//
			unsigned char macadress[32];
			for(unsigned int i =0 ;i < pBSSIDList->NumberOfItems;i++)
			  {
						lvi.iItem =pDlg->m_ctlList.GetItemCount() ;
					lvi.iSubItem = 0;
				  
					int temp=i;
					//step to the next in list...
					PNDIS_WLAN_BSSID cpSsid=pBSSIDList->Bssid;
					while(temp!=0 ){
						cpSsid=(PNDIS_WLAN_BSSID)((char*)cpSsid+ cpSsid->Length);
						temp--;
					}       

			  			lvi.pszText = (LPTSTR)(LPCTSTR)(cpSsid->Ssid.Ssid);
					pDlg->m_ctlList.InsertItem(&lvi);

						lvi.iSubItem = 1;
						
				sprintf((char*)macadress,"%02X-%02X-%02X-%02X-%02X-%02X",(int*)cpSsid->MacAddress[0],(int*)cpSsid->MacAddress[1],
					(int*)pBSSIDList->Bssid[i].MacAddress[2],(int*)cpSsid->MacAddress[3],(int*)cpSsid->MacAddress[4],(int*)cpSsid->MacAddress[5]);
				
							
						lvi.pszText = (LPTSTR)(LPCTSTR)(macadress);
						pDlg->m_ctlList.SetItem(&lvi);
			
						lvi.iSubItem = 2;
						strItem.Format(_T("%i"), cpSsid->Rssi);
						lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
						pDlg->m_ctlList.SetItem(&lvi);

						
						char encryption[32];
						switch(cpSsid->Privacy){
							case 1:
								strcpy(encryption,"WEP");
								break;

							default:
								itoa(cpSsid->Privacy,encryption,10);
						}


						lvi.iSubItem = 3;
						lvi.pszText = (LPTSTR)(LPCTSTR)(encryption);
						pDlg->m_ctlList.SetItem(&lvi);

						lvi.iSubItem = 4;
						int freq= cpSsid->Configuration.DSConfig;
						freq -=2407000;
						freq/=5000;
						
						strItem.Format(_T("%i"), freq);
						lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);//frequnezie
						pDlg->m_ctlList.SetItem(&lvi);

						lvi.iSubItem = 5;
						strItem.Format(_T("%i"), cpSsid->Configuration.BeaconPeriod);
						lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
						pDlg->m_ctlList.SetItem(&lvi);
				
			  }

			
			
	
		}
		pDlg->LabelInfo.SetWindowTextA("scanning...");
		Sleep(500);//please wait...
		
	}

	return true;
}
void CwlanscanDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
