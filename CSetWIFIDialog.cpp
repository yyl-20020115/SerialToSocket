// CSetWIFIDialog.cpp: 实现文件
//

#include "pch.h"
#include "SerialToSocket.h"
#include "afxdialogex.h"
#include "CSetWIFIDialog.h"

// CSetWIFIDialog 对话框

IMPLEMENT_DYNAMIC(CSetWIFIDialog, CDialogEx)

CSetWIFIDialog::CSetWIFIDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SET_WIFI, pParent)
	, cli(nullptr)
{
	this->cli = new httplib::Client("http://192.168.4.1");
	if (this->cli != nullptr) {
		auto home = this->cli->Get("/status.html");
		auto about_get = this->cli->Get("/about.html");
	}
}

CSetWIFIDialog::~CSetWIFIDialog()
{
	if (this->cli != nullptr) {
		delete this->cli;
		this->cli = nullptr;
	}
}

void CSetWIFIDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetWIFIDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetWIFIDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, &CSetWIFIDialog::OnBnClickedButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CSetWIFIDialog::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CSetWIFIDialog 消息处理程序


void CSetWIFIDialog::OnBnClickedOk()
{

	auto serial_get = this->cli->Get("/serial_set.html");
	auto serial_post = this->cli->Post("/serial_set.html");
	//baud=115200&databit=3&parity=2&stop=1&rs485=100&split_time=0&uart_time=45&uart_log=0
	auto wifi_get = this->cli->Get("/wifi_set.html");
	auto wifi_post = this->cli->Post("/wifi_set.html");
	//apE=0&apN=Doit_WiFi&apP=&apIP=192.168.4.1&apNM=255.255.255.0&apGW=192.168.4.1&staE=1&staN=SPW&staP=spw147258&dhcpE=0&staIP=192.168.101.101&staNM=255.255.0.0&staGW=192.168.101.1
	auto net_get = this->cli->Get("/net_set.html");
	auto net_post = this->cli->Post("/net_set.html");
	//net_mode=0&tcplPort=9000&tcpIP=192.168.101.24&tcpPort=6000&udplPort=9000&udpBIP=192.168.101.255&udpBPort=9000&udpIP=192.168.101.24&udpPort=6000


	CDialogEx::OnOK();
}


void CSetWIFIDialog::OnBnClickedButtonRestore()
{
	if (this->cli != nullptr) {
		//call this get to restore
		auto restore_get = this->cli->Get("/restore.html");
	}
}


void CSetWIFIDialog::OnBnClickedButtonReset()
{
	if (this->cli != nullptr) {
		//call this get to restart
		auto restart_get = this->cli->Get("/restart.html");
	}
}
