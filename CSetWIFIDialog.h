#pragma once
#include "afxdialogex.h"
#include "afxdisp.h"
#include "httplib.h"
#include "CEXPLORER_WEB_BROWSER.h"


// CSetWIFIDialog 对话框

class CSetWIFIDialog : public CDialogEx
{
public:
	CString URL;

	DECLARE_DYNAMIC(CSetWIFIDialog)

public:
	CSetWIFIDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetWIFIDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SET_WIFI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonRestore();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonConnect();
	virtual BOOL OnInitDialog();
	CComboBox ComPortsList;
	CComboBox BaudRateList;
	CComboBox DataBitsList;
	CComboBox StopBitsList;
	CEdit URL_Edit;
protected:
	void LoadValues();
	void SaveValues();
	bool PostNetSetValues();
	bool PostSerialSetValues();
	bool PostWIFISetValues();
	bool DoPost(const std::string& url, const std::string& file, const std::string& body);

protected:
	CString STATUS_mac;
	CString STATUS_staIP;
	CString STATUS_wifi;
	CString STATUS_apIP;
	CString STATUS_time;

protected:
	CString	NET_net_mode;
	CString	NET_tcplPort;
	CString	NET_tcpIP;
	CString	NET_tcpPort;
	CString	NET_udplPort;
	CString	NET_udpIP;
	CString	NET_udpPort;
	CString	NET_udpBIP;
	CString	NET_udpBPort;
protected:

	CString SERIAL_baud;
	CString SERIAL_databit;
	CString SERIAL_parity;
	CString SERIAL_stop;
	CString SERIAL_rs485;
	CString SERIAL_uart_time;
	CString SERIAL_split_time;
	CString SERIAL_uart_log;

protected:
	CString WIFI_apE;
	CString WIFI_dhcpE;
	CString WIFI_staE;
	CString WIFI_staN;
	CString WIFI_staP;
	CString WIFI_staIP;
	CString WIFI_staGW;
	CString WIFI_staNM;
	CString WIFI_apN;
	CString WIFI_apP;
	CString WIFI_apIP;
	CString WIFI_apNM;
	CString WIFI_apGW;

protected:

	bool ParseJs(const std::string& url,const std::string& file, std::map<std::string, std::string>& map);
	bool ParseJs(const std::string& js, std::map<std::string, std::string>& map);

};
