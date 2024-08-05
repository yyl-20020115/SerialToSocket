// CSetWIFIDialog.cpp: 实现文件
//

#include "pch.h"
#include "SerialToSocket.h"
#include "afxdialogex.h"
#include "CSetWIFIDialog.h"
#include "include/nlohmann/json.hpp"

CHAR* GetFirstByte(CHAR* ps, int length);
bool GetLocalMachineIPAddress(SOCKADDR* pAddress, SOCKADDR* pDNSAddress, SOCKADDR* pDHCPServerAddress);

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

static std::string str_tolower(const std::string& s) {
	std::string r = s;
	std::transform(r.begin(), r.end(), r.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return r;
}
static std::string str_toupper(const std::string& s) {
	std::string r = s;
	std::transform(r.begin(), r.end(), r.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return r;
}
static std::string replace(const std::string& s, const std::string& a, const std::string& b)
{
	std::string str = s;
	size_t oldPos = 0;
	while (str.find(a, oldPos) != s.npos)
	{
		size_t start = str.find(a, oldPos);
		str.replace(start, a.size(), b);
		oldPos = start + b.size();
	}
	return str;
}
static bool split(const std::string& s, const char c, std::vector<std::string>& list) {
	std::string::size_type begin = 0, end = 0;
	while (s.npos != (end = s.find(c, begin)))
	{
		list.push_back(s.substr(begin, end-begin));
		begin = end + 1;
	}
	//use the whole string s if no c found
	if (list.size() == 0) {
		list.push_back(s);
	}
	return list.size() > 0;
}
static bool StringStartsWith(const std::string& s, const std::string& t, bool case_sensitive = true) {
	return
		case_sensitive
		? s.substr(0, t.size()) == t
		: str_tolower(s.substr(0, t.size())) == str_tolower(t);
}
static bool StringEndsWith(const std::string& s, const std::string& t, bool case_sensitive = true) {
	return
		case_sensitive
		? s.substr(s.size() - t.size()) == t
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
	DDX_Control(pDX, IDC_COMBO_COM_PORTS_LIST, ComPortsList);
	DDX_Control(pDX, IDC_COMBO_BAUD_RATE_LIST, BaudRateList);
	DDX_Control(pDX, IDC_COMBO_DATA_BITS_LIST, DataBitsList);
	DDX_Control(pDX, IDC_COMBO_STOP_BITS_LIST, StopBitsList);
}


BEGIN_MESSAGE_MAP(CSetWIFIDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetWIFIDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, &CSetWIFIDialog::OnBnClickedButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CSetWIFIDialog::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDCANCEL, &CSetWIFIDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSetWIFIDialog::OnBnClickedButtonConnect)
END_MESSAGE_MAP()

void CSetWIFIDialog::OnBnClickedOk()
{
	//baud=115200&databit=3&parity=2&stop=1&rs485=100&split_time=0&uart_time=45&uart_log=0
	//apE=0&apN=Doit_WiFi&apP=&apIP=192.168.4.1&apNM=255.255.255.0&apGW=192.168.4.1&staE=1&staN=SPW&staP=spw147258&dhcpE=0&staIP=192.168.101.101&staNM=255.255.0.0&staGW=192.168.101.1
	//net_mode=0&tcplPort=9000&tcpIP=192.168.101.24&tcpPort=6000&udplPort=9000&udpBIP=192.168.101.255&udpBPort=9000&udpIP=192.168.101.24&udpPort=6000
	this->SaveValues();
	CDialogEx::OnOK();
}


void CSetWIFIDialog::OnBnClickedButtonRestore()
{
	httplib::Client* cli = new httplib::Client(((CStringA)URL).GetString());

	if (cli != nullptr) {
		//call this get to restore
		auto restore_get = cli->Get("/restore.html");
		if (restore_get != nullptr && restore_get->status == httplib::StatusCode::OK_200) {

		}
		delete cli;
	}
}


void CSetWIFIDialog::OnBnClickedButtonReset()
{
	httplib::Client* cli = new httplib::Client(((CStringA)URL).GetString());
	if (cli != nullptr) {
		//call this get to restart
		auto restart_get = cli->Get("/restart.html");
		if (restart_get != nullptr && restart_get->status == httplib::StatusCode::OK_200) {

		}
		delete cli;
	}
}


void CSetWIFIDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CSetWIFIDialog::OnBnClickedButtonConnect()
{
	this->LoadValues();
}

static int BaudRates[] = {
	3686400,
	1843200,
	921600,
	460800,
	230400,
	115200,
	74880,
	57600,
	38400,
	19200,
	9600,
	4800,
	2400,
	1200,
	600,
	300,
};

BOOL CSetWIFIDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->URL_Edit.SetWindowText(URL);



	SOCKADDR address = { 0 }, dns_address = { 0 }, dhcp_address = { 0 };

	if (GetLocalMachineIPAddress(&address, &dns_address, &dhcp_address))
	{
		WCHAR IPdotdec[20] = { 0 };
		CHAR* pc = GetFirstByte(address.sa_data, sizeof(address.sa_data));
		PCWCHAR pwchar = (pc != 0)
			? InetNtopW(AF_INET, pc, IPdotdec, sizeof(IPdotdec) / sizeof(wchar_t))
			: 0
			;
		if (pwchar != NULL) {

		}

	}



	return TRUE;
}

void CSetWIFIDialog::LoadValues()
{
	this->URL_Edit.GetWindowText(this->URL);
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
	// http://192.168.4.1/sta_scan.js
	// scanCallBack({'scan':'Mate 20 X$ChinaNet-t6Vd$ChinaNet-t6Vd-iTV$CU_FaQE$DIRECT-64-HP M233 LaserJet$TP-LINK_599D$FAST_2.4G_212F$GUOPAI$一只小可爱$OPPO A11x$'})
	std::vector<std::string> list;
	std::map<std::string, std::string> map;
	ParseScanCallBack(CStringA(this->URL).GetString(), ("/sta_scan.js"), list);
	
	if (ParseParametersCallBack(CStringA(this->URL).GetString(), ("/status_para.js"), map)
		&& ParseParametersCallBack(CStringA(this->URL).GetString(), ("/serial_para.js"), map)
		&& ParseParametersCallBack(CStringA(this->URL).GetString(), ("/sta_para.js"), map)
		&& ParseParametersCallBack(CStringA(this->URL).GetString(), ("/net_para.js"), map)
		) {
		this->STATUS_mac = CString(map[("mac")].c_str());
		this->STATUS_staIP = CString(map[("staIP")].c_str());
		this->STATUS_wifi = CString(map[("wifi")].c_str());
		this->STATUS_apIP = CString(map[("apIP")].c_str());
		this->STATUS_time = CString(map[("time")].c_str());
		this->SERIAL_baud = CString(map[("baud")].c_str());
		this->SERIAL_databit = CString(map[("databit")].c_str());
		this->SERIAL_parity = CString(map[("parity")].c_str());
		this->SERIAL_stop = CString(map[("stop")].c_str());
		this->SERIAL_rs485 = CString(map[("rs485")].c_str());
		this->SERIAL_uart_time = CString(map[("uart_time")].c_str());
		this->SERIAL_split_time = CString(map[("split_time")].c_str());
		this->SERIAL_uart_log = CString(map[("uart_log")].c_str());
		this->NET_net_mode = CString(map[("net_mode")].c_str());
		this->NET_tcplPort = CString(map[("tcplPort")].c_str());
		this->NET_tcpIP = CString(map[("tcpIP")].c_str());
		this->NET_tcpPort = CString(map[("tcpPort")].c_str());
		this->NET_udplPort = CString(map[("udplPort")].c_str());
		this->NET_udpIP = CString(map[("udpIP")].c_str());
		this->NET_udpPort = CString(map[("udpPort")].c_str());
		this->NET_udpBIP = CString(map[("udpBIP")].c_str());
		this->NET_udpBPort = CString(map[("udpBPort")].c_str());
		this->WIFI_staN = CString(map[("staN")].c_str());
		this->WIFI_staP = CString(map[("staP")].c_str());
		this->WIFI_staIP = CString(map[("staIP")].c_str());
		this->WIFI_staGW = CString(map[("staGW")].c_str());
		this->WIFI_staNM = CString(map[("staNM")].c_str());
		this->WIFI_apN = CString(map[("apN")].c_str());
		this->WIFI_apP = CString(map[("apP")].c_str());
		this->WIFI_apIP = CString(map[("apIP")].c_str());
		this->WIFI_apNM = CString(map[("apNM")].c_str());
		this->WIFI_apGW = CString(map[("apGW")].c_str());
	}
}

void CSetWIFIDialog::SaveValues()
{
	this->PostSerialSetValues();
	this->PostWIFISetValues();
	this->PostNetSetValues();
}


bool CSetWIFIDialog::PostSerialSetValues()
{
	// baud=9600
	// &databit=3
	// &parity=2
	// &stop=1
	// &rs485=100
	// &split_time=0
	// &uart_time=50
	// &uart_log=0
	CString post;
	post.Format(
		_T("baud=%s&databit=%s&parity=%s&stop=%s&rs485=%s&split_time=%s&uart_time=%s&uart_log=%s"),
		this->SERIAL_baud,
		this->SERIAL_databit,
		this->SERIAL_parity,
		this->SERIAL_stop,
		this->SERIAL_rs485,
		this->SERIAL_split_time,
		this->SERIAL_uart_time,
		this->SERIAL_uart_log);

	return DoPost(CStringA(this->URL).GetString(), "/serial_set.html", CStringA(post).GetString());
}
bool CSetWIFIDialog::PostWIFISetValues()
{
	// apE=1
	// &apN=Doit_WiFi
	// &apP=
	// &apIP=192.168.4.1
	// &apNM=255.255.255.0
	// &apGW=192.168.4.1
	// &staE=0
	// &staN=Wireless_Router
	// &staP=
	// &dhcpE=1
	// &staIP=192.168.1.1
	// &staNM=255.255.255.0
	// &staGW=192.168.1.1
	CString post;
	post.Format(
		_T("apE=%s&apN=%s&apP=%s&apIP=%s&apNM=%s&apGW=%s&staE=%s&staN=%s&staP=%s&dhcpE=%s&staIP=%s&staNM=%s&staGW=%s"),
		this->WIFI_apE,
		this->WIFI_apN,
		this->WIFI_apP,
		this->WIFI_apIP,
		this->WIFI_apNM,
		this->WIFI_apGW,
		this->WIFI_staE,
		this->WIFI_staN,
		this->WIFI_staP,
		this->WIFI_dhcpE,
		this->WIFI_staIP,
		this->WIFI_staNM,
		this->WIFI_staGW
	);

	return DoPost(CStringA(this->URL).GetString(), "/wifi_set.html", CStringA(post).GetString());
}
bool CSetWIFIDialog::PostNetSetValues()
{
	// net_mode=0
	// &tcplPort=9000
	// &tcpIP=192.168.1.100
	// &tcpPort=6000
	// &udplPort=9000
	// &udpBIP=192.168.1.255
	// &udpBPort=9000
	// &udpIP=192.168.1.100
	// &udpPort=6000
	CString post;
	post.Format(
		_T("net_mode=%s&tcplPort=%s&tcpIP=%s&tcpPort=%s&udplPort=%s&udpBIP=%s&udpBPort=%s&udpIP=%s&udpPort=%s"),
		this->NET_net_mode,
		this->NET_tcplPort,
		this->NET_tcpIP,
		this->NET_tcpPort,
		this->NET_udplPort,
		this->NET_udpBIP,
		this->NET_udpBPort,
		this->NET_udpIP,
		this->NET_udpPort
	);

	return DoPost(CStringA(this->URL).GetString(), "/net_set.html", CStringA(post).GetString());
}

bool CSetWIFIDialog::DoPost(const std::string& url, const std::string& file, const std::string& body)
{
	bool done = false;
	httplib::Client* cli = new httplib::Client(url);

	if (cli != nullptr) {
		std::string content_type = "text/html";
		//call this get to restore
		auto post_action = cli->Post(file, body, content_type);
		done = (post_action != nullptr && post_action->status == httplib::StatusCode::OK_200);
		delete cli;
	}
	return done;
}
bool CSetWIFIDialog::ParseParametersCallBack(const std::string& url, const std::string& file, std::map<std::string, std::string>& map)
{
	bool done = false;
	httplib::Client* cli = new httplib::Client(url);
	if (cli != nullptr) {
		//call this get to restore
		auto http_get = cli->Get(file);
		if (http_get && http_get->status == httplib::StatusCode::OK_200) {
			done = this->ParseParametersCallBack(http_get->body.c_str(), map);
		}
		delete cli;
	}
	return done;
}

bool CSetWIFIDialog::ParseParametersCallBack(const std::string& js, std::map<std::string, std::string>& map)
{
	// paraCallBack({'softver':'Doit_Ser2Sock_3.2.1_20171229','hardver':'Doit_WiFi_TTL_V2.0','ID':'4B696E636F','vend':'Doit Corp. Inc.','mac':'84-0D-8E-4E-33-36','staIP':'0.0.0.0','wifi':'un known','apIP':'192.168.4.1','time':'0 days 02:36:18'})
	if (StringStartsWith(js, "paraCallBack(") && StringEndsWith(js, ")"))
	{
		size_t sl = strlen("paraCallBack(");
		size_t st = strlen(")");
		std::string json_text = js.substr(sl, js.size() - sl - st);
		json_text = replace(json_text, "\'", "\"");
		std::stringstream fs(json_text);
		try {
			nlohmann::json data = nlohmann::json::parse(fs);
			for (auto p = data.begin(); p != data.end(); p++) {
				std::string key = p.key();
				std::string dump = p.value().dump();
				if (dump[0] == '\"' && dump[dump.size() - 1] == '\"') {
					dump = dump.substr(1, dump.size() - 2);
				}
				map[key] = dump;
			}
			return true;
		}
		catch (...) {

		}
	}
	return false;
}

bool CSetWIFIDialog::ParseScanCallBack(const std::string& url, const std::string& file, std::vector<std::string>& list)
{
	bool done = false;
	httplib::Client* cli = new httplib::Client(url);
	if (cli != nullptr) {
		//call this get to restore
		auto http_get = cli->Get(file);
		if (http_get && http_get->status == httplib::StatusCode::OK_200) {
			done = this->ParseScanCallBack(http_get->body.c_str(), list);
		}
		delete cli;
	}
	return done;
}
// scanCallBack({'scan':'Mate 20 X$ChinaNet-t6Vd$ChinaNet-t6Vd-iTV$CU_FaQE$DIRECT-64-HP M233 LaserJet$TP-LINK_599D$FAST_2.4G_212F$GUOPAI$一只小可爱$OPPO A11x$'})
bool CSetWIFIDialog::ParseScanCallBack(const std::string& js, std::vector<std::string>& list)
{
	if (StringStartsWith(js, "scanCallBack(") && StringEndsWith(js, ")"))
	{
		size_t sl = strlen("scanCallBack(");
		size_t st = strlen(")");
		std::string json_text = js.substr(sl, js.size() - sl - st);
		json_text = replace(json_text, "\'", "\"");
		std::stringstream fs(json_text);
		try {
			const char splitter = '$';
			nlohmann::json data = nlohmann::json::parse(fs);
			auto& scan = data["scan"];
			if (scan.is_string()) {
				std::string dump = scan.dump();
				if (dump.size() >= 2) {
					dump = dump.substr(1, dump.size() - 2);
				}
				//string split by '$'
				return split(dump,splitter,list);
			}
			return true;
		}
		catch (...) {

		}
	}
	return false;
}
