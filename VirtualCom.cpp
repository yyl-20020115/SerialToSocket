#include "pch.h"
#include "VirtualCom.h"
#include "FactoryPreset.h"
#include <windows.h>
#include <iostream>
#include <filesystem>
#include "Utils.h"

VirtualCom::VirtualCom()
{

}

/**
* 加载虚拟串口列表
**/
void VirtualCom::loadCommPairs(CommPairType* commPairs, size_t* numOfPairs)
{
    uint8_t pariCount = 0;
    uint8_t i = 0;
    long cvtIndex = 0;

    char* resp = new char[1024];
    memset(resp, 0, sizeof(char)*1024);

    FactoryPreset preset;
    
    preset.LoadConfig();

    std::wstring cmdStr = preset.vcom_driver_path + L" list";

    execReadBack(cmdStr, resp, 1024);
    cout << resp << endl;
    //-------------------------------------
    std::string readBuff = resp;
    
    for (i = 0; i < 32; i++) {
        size_t p = 0;
        p = readBuff.find("\r\n = ", p);
        if (p!=readBuff.npos) {
            std::string pars = readBuff.substr(0, p);
            if (pars.substr(0,2) == ("CN")) {
                //cout<<"Find Pair: "<<pars.substr(4,1)<<endl;
                if (sscanf("%d",pars.substr(4, 1).c_str(), &cvtIndex)) {
                    commPairs[pariCount].index = (uint8_t)cvtIndex;
                }
                readBuff = readBuff.substr(p);
                std::string port = readBuff;
                if (pars.substr(3, 1) == ("A")) {
                    commPairs[pariCount].comA = string_to_wstring(port);
                    //cout<<"PortA: "<<port<<endl;
                }
                if (pars.substr(3, 1) == ("B")) {
                    commPairs[pariCount].comB = string_to_wstring(port);
                    //cout<<"PortB: "<<port<<endl;

                    pariCount++;
                }
            }
        }
        else {
            break;
        }
    }

    //输出数量
    *(size_t*)numOfPairs = pariCount;
    //-------------------------------------
    delete[] resp;
}

/**
* 创建一对新的虚拟串口
**/
void VirtualCom::createComPair(const std::wstring& comportA, const std::wstring& comportB)
{
    FactoryPreset preset;
    std::wstring cmdStr = preset.vcom_driver_path + L" install PortName=" + comportA + L" PortName=" + comportB;
    char* resp = new char[1024];
    memset(resp, 0, 1024);
    execReadBack(cmdStr, resp, 1024);
    delete[] resp;
}

/**
* 删除指定的一对虚拟串口
**/
void VirtualCom::removeComPair(uint8_t pairIndex)
{
    FactoryPreset preset;
    std::wstring cmdStr = preset.vcom_driver_path + L" remove " + std::to_wstring(pairIndex);
    char* resp = new char[1024];
    memset(resp, 0, 1024);
    execReadBack(cmdStr, resp, 1024);
    delete[] resp;
}

/**
* 安装虚拟串口驱动
**/
uint8_t VirtualCom::installDriver()
{
    uint8_t ret = 0;
    FactoryPreset preset;
    std::wstring cmdStr = preset.vcom_driver_path + L" preinstall";

    char* resp = new char[1024];
    memset(resp, 0, 1024);
    ret = execReadBack(cmdStr, resp, 1024);
    cout << resp << endl;
    delete[] resp;
    return ret;
}

/**
* 执行一个指令并读取其命令行响应
* 传入的命令必须加.exe
**/
uint8_t VirtualCom::execReadBack(const std::wstring& cmd, char* read_buff, size_t buff_len)
{
    std::wstring dirPath;
    size_t p = cmd.find(_T(" "));
    if (p!=cmd.npos) {
        std::wstring exePath = cmd.substr(p+1);
        //cout<<"First: "<<exePath<<endl;
        //获取指定命令的运行目录
        if (filesystem::exists(exePath))
        {
            p = exePath.rfind(_T("\\"));
            if (p != exePath.npos) {
                dirPath = exePath.substr(0,p);
            }
        }
        else {
            return -1;
        }
    }
    //---------------------------------------
    DWORD bytesRead;
    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
    {
        return -1;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfo(&si);
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    TCHAR wchar_cmd[256];
    wcscpy(wchar_cmd, cmd.c_str());

    TCHAR wchar_path[256];
    wcscpy(wchar_path, dirPath.c_str());

    if (!CreateProcess(NULL, wchar_cmd, NULL, NULL, TRUE, 0, NULL, wchar_path, &si, &pi))
    {
        return -1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    //printf("Process Terminate !\r\n");

    CloseHandle(hWrite);
    //printf("Get Output OK !\r\n");

    ReadFile(hRead, read_buff, buff_len, &bytesRead, NULL);
    return 0;
}

VirtualCom::~VirtualCom()
{
    //dtor
}
