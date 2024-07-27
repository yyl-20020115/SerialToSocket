// CSetWIFIDialog.cpp: 实现文件
//

#include "pch.h"
#include "SerialToSocket.h"
#include "afxdialogex.h"
#include "CSetWIFIDialog.h"
#include "include/nlohmann/json.hpp"
#include <MsHTML.h>

static bool StringEndsWith(const CString& s, const CString& t, bool case_sensitive = true) {
	return
		case_sensitive
		? s.Right(t.GetLength()) == t
		: s.Right(t.GetLength()).MakeLower() == CString(t).MakeLower();
}
static bool StringStartsWith(const CString& s, const CString& t, bool case_sensitive = true) {
	return
		case_sensitive
		? s.Left(t.GetLength()) == t
		: s.Left(t.GetLength()).MakeLower() == CString(t).MakeLower();
}
static bool StringStartsWith(const CStringA& s, const CStringA& t, bool case_sensitive = true) {
	return
		case_sensitive
		? s.Left(t.GetLength()) == t
		: s.Left(t.GetLength()).MakeLower() == CStringA(t).MakeLower();
}
static bool StringEndsWith(const CStringA& s, const CStringA& t, bool case_sensitive = true) {
	return
		case_sensitive
		? s.Right(t.GetLength()) == t
		: s.Right(t.GetLength()).MakeLower() == CStringA(t).MakeLower();
}

std::string str_tolower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); } 
	);
	return s;
}
std::string replace(std::string str, const std::string& a, const std::string& b)
{
	int oldPos = 0;
	while (str.find(a, oldPos) != -1)
	{
		int start = str.find(a, oldPos);
		str.replace(start, a.size(), b);
		oldPos = start + b.size();
	}
	return str;
}

static bool StringStartsWith(const std::string& s, const std::string& t, bool case_sensitive = true) {
	return
		case_sensitive
		? s.substr(0,t.size()) == t
		: str_tolower(s.substr(0,t.size())) == str_tolower(t);
}
static bool StringEndsWith(const std::string& s, const std::string& t, bool case_sensitive = true) {
	return
		case_sensitive
		? s.substr(s.size()-t.size()) == t
		: str_tolower(s.substr(s.size() - t.size())) == str_tolower(t);
}

IMPLEMENT_DYNAMIC(CSetWIFIDialog, CDialogEx)

CSetWIFIDialog::CSetWIFIDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SET_WIFI, pParent)
	, URL(_T("http://192.168.4.1"))
{
}

CSetWIFIDialog::~CSetWIFIDialog()
{
}

void CSetWIFIDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IP, URL_Edit);
	DDX_Control(pDX, IDC_EXPLORER_WEB_BROWSER, WebBrowser);
	DDX_Control(pDX, IDC_COMBO_COM_PORTS_LIST, COMPortList);
	DDX_Control(pDX, IDC_COMBO_BAUD, BaudRateList);
	DDX_Control(pDX, IDC_COMBO_DATA_BITS, DataBitsList);
	DDX_Control(pDX, IDC_COMBO_STOP_BITS_LIST, StopBitsList);
}


BEGIN_MESSAGE_MAP(CSetWIFIDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetWIFIDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, &CSetWIFIDialog::OnBnClickedButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CSetWIFIDialog::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDCANCEL, &CSetWIFIDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSetWIFIDialog::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CSetWIFIDialog 消息处理程序


void CSetWIFIDialog::OnBnClickedOk()
{

	//baud=115200&databit=3&parity=2&stop=1&rs485=100&split_time=0&uart_time=45&uart_log=0
	//apE=0&apN=Doit_WiFi&apP=&apIP=192.168.4.1&apNM=255.255.255.0&apGW=192.168.4.1&staE=1&staN=SPW&staP=spw147258&dhcpE=0&staIP=192.168.101.101&staNM=255.255.0.0&staGW=192.168.101.1
	//net_mode=0&tcplPort=9000&tcpIP=192.168.101.24&tcpPort=6000&udplPort=9000&udpBIP=192.168.101.255&udpBPort=9000&udpIP=192.168.101.24&udpPort=6000


	CDialogEx::OnOK();
}


void CSetWIFIDialog::OnBnClickedButtonRestore()
{
	httplib::Client* cli = new httplib::Client(((CStringA)URL).GetString());

	if (cli != nullptr) {
		//call this get to restore
		auto restore_get = cli->Get("/restore.html");
		delete cli;
	}
}


void CSetWIFIDialog::OnBnClickedButtonReset()
{
	httplib::Client* cli = new httplib::Client(((CStringA)URL).GetString());
	if (cli != nullptr) {
		//call this get to restart
		auto restart_get = cli->Get("/restart.html");
		delete cli;
	}
}


void CSetWIFIDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CSetWIFIDialog::OnBnClickedButtonConnect()
{
	this->GetStatusValues();
	//this->URL_Edit.GetWindowText(this->URL);
	//this->WebBrowser.Navigate(this->URL + _T("/serial_set.html"), NULL, NULL, NULL, NULL);
}


BOOL CSetWIFIDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->URL_Edit.SetWindowText(URL);
	this->WebBrowser.put_Silent(TRUE);
	return TRUE;
}

CString CSetWIFIDialog::GetTextFromWebBrowser(CEXPLORER_WEB_BROWSER& WebBrowser,CString name)
{
	HRESULT hr = E_FAIL;
	IHTMLDocument3* pDoc3 = NULL;
	CComQIPtr <IHTMLDocument2, &IID_IHTMLDocument2> doc = this->WebBrowser.get_Document();
	if (doc != nullptr && S_OK == (hr = doc->QueryInterface(IID_IHTMLDocument3, (void**)&pDoc3))
		&& pDoc3 != NULL) {
		CComPtr <IHTMLElement> pUserElement;
		CComBSTR idName(CT2OLE(name.GetString()));
		hr = pDoc3->getElementById(idName, &pUserElement);
		if (FAILED(hr) || !pUserElement) return _T("");

		CComPtr <IHTMLInputElement> input;
		if (S_OK == (hr = pUserElement->QueryInterface(&input)))
		{
			CComBSTR text;
			if (S_OK == (hr = input->get_value(&text)))
			{
				return text;
			}
		}
	}
	return _T("");
}

void CSetWIFIDialog::WaitForDocumentComplete()
{
}

void CSetWIFIDialog::GetStatusValues()
{
	std::map<std::string,std::string> map;
	if (ParseJs(CStringA(this->URL).GetString(), ("/status_para.js"), map)
		&& ParseJs(CStringA(this->URL).GetString(), ("/serial_para.js"), map)
		&& ParseJs(CStringA(this->URL).GetString(), ("/sta_para.js"), map)
		&& ParseJs(CStringA(this->URL).GetString(), ("/net_para.js"), map)
		) {

	}
}


void CSetWIFIDialog::GetSerialSetValues()
{
}

void CSetWIFIDialog::GetWIFISetValues()
{
}
void CSetWIFIDialog::GetNetSetValues()
{
}
BEGIN_EVENTSINK_MAP(CSetWIFIDialog, CDialogEx)
ON_EVENT(CSetWIFIDialog, IDC_EXPLORER_WEB_BROWSER, 259, CSetWIFIDialog::OnDocumentcompleteExplorerWebBrowser, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

// version
// http://192.168.4.1/about.html
// restore
// http://192.168.4.1/restore.html
// restart
// http://192.168.4.1/restart.html
//
// http://192.168.4.1/net_para.js
// paraCallBack({'net_mode':'0','tcplPort':'9000','tcpIP':'192.168.1.100','tcpPort':'6000','udplPort':'9000','udpBIP':'192.168.1.255','udpBPort':'9000','udpIP':'192.168.1.100','udpPort':'6000'})
// http://192.168.4.1/sta_para.js
// paraCallBack({ 'staE':'staE0','staN' : 'Wireless_Router','staP' : '','dhcpE' : 'dhcpE1','staIP' : '192.168.1.1','staNM' : '255.255.255.0','staGW' : '192.168.1.1','apE' : 'apE1','apN' : 'Doit_WiFi','apP' : '','apIP' : '192.168.4.1','apNM' : '255.255.255.0','apGW' : '192.168.4.1' })
// http://192.168.4.1/serial_para.js
// paraCallBack({'baud':'9600','databit':'3','parity':'2','stop':'1','rs485':'100','uart_time':'50','split_time':'0','uart_log':'0'})
// http://192.168.4.1/status_para.js
// paraCallBack({'softver':'Doit_Ser2Sock_3.2.1_20171229','hardver':'Doit_WiFi_TTL_V2.0','ID':'4B696E636F','vend':'Doit Corp. Inc.','mac':'84-0D-8E-4E-33-36','staIP':'0.0.0.0','wifi':'un known','apIP':'192.168.4.1','time':'0 days 02:36:18'})

void CSetWIFIDialog::OnDocumentcompleteExplorerWebBrowser(LPDISPATCH pDisp, VARIANT* URL)
{
	CString _URL(*URL);

	if (StringEndsWith(_URL, _T("/status.html")))
	{
		this->STATUS_mac = this->GetTextFromWebBrowser(this->WebBrowser, _T("mac"));
		this->STATUS_staIP = this->GetTextFromWebBrowser(this->WebBrowser, _T("staIP"));
		this->STATUS_wifi = this->GetTextFromWebBrowser(this->WebBrowser, _T("wifi"));
		this->STATUS_apIP = this->GetTextFromWebBrowser(this->WebBrowser, _T("apIP"));
		this->STATUS_time = this->GetTextFromWebBrowser(this->WebBrowser, _T("time"));
		this->WebBrowser.Navigate(this->URL + "/serial_set.html", NULL, NULL, NULL, NULL);
	}
	else if (StringEndsWith(_URL, _T("/serial_set.html"))) {
		this->SERIAL_baud = this->GetTextFromWebBrowser(this->WebBrowser, _T("baud"));
		this->SERIAL_databit = this->GetTextFromWebBrowser(this->WebBrowser, _T("databit"));
		this->SERIAL_parity = this->GetTextFromWebBrowser(this->WebBrowser, _T("parity"));
		this->SERIAL_stop = this->GetTextFromWebBrowser(this->WebBrowser, _T("stop"));
		this->SERIAL_rs485 = this->GetTextFromWebBrowser(this->WebBrowser, _T("rs485"));
		this->SERIAL_uart_time = this->GetTextFromWebBrowser(this->WebBrowser, _T("uart_time"));
		this->SERIAL_split_time = this->GetTextFromWebBrowser(this->WebBrowser, _T("split_time"));
		this->SERIAL_uart_log = this->GetTextFromWebBrowser(this->WebBrowser, _T("uart_log"));
		this->WebBrowser.Navigate(this->URL + "/net_set.html", NULL, NULL, NULL, NULL);
	}
	else if (StringEndsWith(_URL, _T("/net_set.html"))) {
		this->NET_net_mode = this->GetTextFromWebBrowser(this->WebBrowser, _T("net_mode"));
		this->NET_tcplPort = this->GetTextFromWebBrowser(this->WebBrowser, _T("tcplPort"));
		this->NET_tcpIP = this->GetTextFromWebBrowser(this->WebBrowser, _T("tcpIP"));
		this->NET_tcpPort = this->GetTextFromWebBrowser(this->WebBrowser, _T("tcpPort"));
		this->NET_udplPort = this->GetTextFromWebBrowser(this->WebBrowser, _T("udplPort"));
		this->NET_udpIP = this->GetTextFromWebBrowser(this->WebBrowser, _T("udpIP"));
		this->NET_udpPort = this->GetTextFromWebBrowser(this->WebBrowser, _T("udpPort"));
		this->NET_udpBIP = this->GetTextFromWebBrowser(this->WebBrowser, _T("udpBIP"));
		this->NET_udpBPort = this->GetTextFromWebBrowser(this->WebBrowser, _T("udpBPort"));
		this->WebBrowser.Navigate(this->URL + "/wifi_set.html", NULL, NULL, NULL, NULL);
	}
	else if (StringEndsWith(_URL, _T("/wifi_set.html"))) {
		this->WIFI_staN = this->GetTextFromWebBrowser(this->WebBrowser, _T("staN"));
		this->WIFI_staP = this->GetTextFromWebBrowser(this->WebBrowser, _T("staP"));
		this->WIFI_staIP = this->GetTextFromWebBrowser(this->WebBrowser, _T("staIP"));
		this->WIFI_staGW = this->GetTextFromWebBrowser(this->WebBrowser, _T("staGW"));
		this->WIFI_staNM = this->GetTextFromWebBrowser(this->WebBrowser, _T("staNM"));
		this->WIFI_apN = this->GetTextFromWebBrowser(this->WebBrowser, _T("apN"));
		this->WIFI_apP = this->GetTextFromWebBrowser(this->WebBrowser, _T("apP"));
		this->WIFI_apIP = this->GetTextFromWebBrowser(this->WebBrowser, _T("apIP"));
		this->WIFI_apNM = this->GetTextFromWebBrowser(this->WebBrowser, _T("apNM"));
		this->WIFI_apGW = this->GetTextFromWebBrowser(this->WebBrowser, _T("apGW"));
	}

	this->DocumentCompleted = TRUE;
}

bool CSetWIFIDialog::ParseJs(const std::string& url, const std::string& file, std::map<std::string,std::string>& map)
{
	bool done = false;

	httplib::Client* cli = new httplib::Client(url);

	if (cli != nullptr) {
		//call this get to restore
		auto restore_get = cli->Get(file);
		if (restore_get->status == httplib::StatusCode::OK_200) {
			done = this->ParseJs(restore_get->body.c_str(), map);
		}
		delete cli;
	}
	return done;
}

bool CSetWIFIDialog::ParseJs(const std::string& js, std::map<std::string, std::string>& map)
{
	// paraCallBack({'softver':'Doit_Ser2Sock_3.2.1_20171229','hardver':'Doit_WiFi_TTL_V2.0','ID':'4B696E636F','vend':'Doit Corp. Inc.','mac':'84-0D-8E-4E-33-36','staIP':'0.0.0.0','wifi':'un known','apIP':'192.168.4.1','time':'0 days 02:36:18'})
	if(StringStartsWith(js, "paraCallBack(") && StringEndsWith(js,")"))
	{
		std::string json_text = js.substr(13, js.size() - 14);
		json_text = replace(json_text,"\'", "\"");
		std::stringstream fs(json_text);
		nlohmann::json data = nlohmann::json::parse(fs);
		for (auto p = data.begin(); p != data.end();p++) {
			std::string key = p.key();
			auto value = p.value();
			{
				map[(key.c_str())] = (value.dump().c_str());
			}
		}
		return true;

	}
	return false;
}
