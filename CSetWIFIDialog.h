#pragma once
#include "afxdialogex.h"
#include "httplib.h"


// CSetWIFIDialog 对话框

class CSetWIFIDialog : public CDialogEx
{
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


private:

	httplib::Client* cli;
public:
	afx_msg void OnBnClickedButtonRestore();
	afx_msg void OnBnClickedButtonReset();
};
