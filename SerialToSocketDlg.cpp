#include "pch.h"
#include "framework.h"
#include "SerialToSocket.h"
#include "SerialToSocketDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CSetWIFIDialog.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

#define WORKING_BUFFER_SIZE 15000

#define HEAP_MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define HEAP_FREE(x) HeapFree(GetProcessHeap(), 0, (x))


CHAR* GetFirstByte(CHAR* ps, int length) {
	CHAR* pc = ps;
	for (; pc != ps + length; pc++)
		if (*pc != 0) break;

	return pc == ps + length ? 0 : pc;
}
/* Note: could also use malloc() and free() */
bool GetLocalMachineIPAddress(SOCKADDR* pAddress, SOCKADDR* pDNSAddress = 0, SOCKADDR* pDHCPServerAddress = 0)
{

	DWORD dwRetVal = 0;
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
	ULONG family = AF_INET;
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	pAddresses = (IP_ADAPTER_ADDRESSES*)HEAP_MALLOC(outBufLen);
	if (pAddresses == NULL) return false;

	memset(pAddresses, 0, sizeof(IP_ADAPTER_ADDRESSES));

	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if (pAddress != NULL) {
		*pAddress = *pAddresses->FirstUnicastAddress->Address.lpSockaddr;
	}
	if (pDNSAddress != NULL && pAddresses->FirstDnsServerAddress !=NULL) {
		*pDNSAddress = *pAddresses->FirstDnsServerAddress->Address.lpSockaddr;
	}

	HEAP_FREE(pAddresses);
	pAddresses = NULL;

	return dwRetVal == NO_ERROR;
}

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
	DDX_Control(pDX, IDC_EDIT_IP, EditIP);
	DDX_Control(pDX, IDC_EDIT_PORT, EditPort);
	DDX_Control(pDX, IDC_COMBO_COM_PORTS_LIST, ComPortsList);
	DDX_Control(pDX, IDC_COMBO_BAUD_RATE_LIST, BaudRateList);
	DDX_Control(pDX, IDC_COMBO_DATA_BITS_LIST, DataBitsList);
	DDX_Control(pDX, IDC_COMBO_PARITIES_LIST, ParitiesList);
	DDX_Control(pDX, IDC_COMBO_STOP_BITS_LIST, StopBitsList);
	DDX_Control(pDX, IDC_EDIT_SSID_PASSWORD, Edit_Password);
	DDX_Control(pDX, IDC_COMBO_SSID, ComboSSIDList);
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

	SOCKADDR address = { 0 };

	if (GetLocalMachineIPAddress(&address))
	{
		WCHAR IPdotdec[20] = { 0 };
		CHAR* pc = GetFirstByte(address.sa_data, 14);
		PCWCHAR pwchar = (pc != 0)
			? InetNtopW(AF_INET, pc, IPdotdec, sizeof(IPdotdec) / sizeof(wchar_t))
			: 0
			;
		if (pwchar != NULL) {
			this->EditIP.SetWindowText(pwchar);
		}
	}
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
