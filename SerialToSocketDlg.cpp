#include "pch.h"
#include "framework.h"
#include "SerialToSocket.h"
#include "SerialToSocketDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CSetWIFIDialog.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CSerialToSocketDlg::CSerialToSocketDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SERIALTOSOCKET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialToSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSerialToSocketDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MAPPING, &CSerialToSocketDlg::OnBnClickedButtonMapping)
	ON_BN_CLICKED(IDC_BUTTON_WIFI_COM, &CSerialToSocketDlg::OnBnClickedButtonWifiCom)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSerialToSocketDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDOK, &CSerialToSocketDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerialToSocketDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CSerialToSocketDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}

void CSerialToSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialToSocketDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSerialToSocketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSerialToSocketDlg::OnBnClickedButtonMapping()
{
}


void CSerialToSocketDlg::OnBnClickedButtonWifiCom()
{
	CSetWIFIDialog dialog;
	INT_PTR result = dialog.DoModal();
	if (result == IDOK) {
		//OK
	}
}


void CSerialToSocketDlg::OnBnClickedButtonStart()
{

}


void CSerialToSocketDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}


void CSerialToSocketDlg::OnBnClickedCancel()
{
	this->ShowWindow(SW_HIDE);
}
