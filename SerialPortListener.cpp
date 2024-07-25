#include "pch.h"
#include "SerialPortListener.h"
#include "SerialCommService.h"
#include "FactoryPreset.h"

SerialPortListener::SerialPortListener(SerialCommService* service, HANDLE hCom, uint8_t* recv_buff, const size_t recv_len) :
    hComPort(hCom),
    RECV_BUFF(recv_buff),
    BUFF_SIZE(recv_len),
    isQuit(false),
    service(service)
{
}

/**
*   具体的线程函数
**/
int SerialPortListener::Entry()
{
    isQuit = false;
    while (::WaitForSingleObject(this->hComPort,!10) && !isQuit)
    {
        /**
        *   避免过多的占用系统CPU
        *   这里加一点休眠释放一点时间片
        *   取值应在10ms至100ms之间,取决于每两次接收数据的间隔
        *   如果取消掉该延迟会导致CPU占用率爆满
        **/

        //开始读取串口数据
        if (!isQuit)
        {
            //清空缓冲区并重新读取
            memset(RECV_BUFF, 0, BUFF_SIZE);
            ReadComPort(RECV_BUFF, BUFF_SIZE);
        }
    }
    Delete();

    return 0;
}

/**
*   从串口读取数据
**/
int SerialPortListener::ReadComPort(uint8_t* read_buff, unsigned long readLen) const
{
    DWORD i = 0;
    DWORD dwBytesRead = readLen;
    BOOL bReadRet;
    DWORD dwErrorFlag;
    COMSTAT statCom;
    OVERLAPPED ovlpRead;

    ClearCommError(hComPort, &dwErrorFlag, &statCom);
    if (!statCom.cbInQue)
    {
        return -1;			//读取缓冲区中没有数据
    }
    memset(&ovlpRead, 0, sizeof(OVERLAPPED));
    ovlpRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    bReadRet = ReadFile(hComPort, read_buff, readLen, &dwBytesRead, &ovlpRead);
    if (!bReadRet)			//在异步串口中,ReadFile返回FALSE不一定就是读取错误
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            GetOverlappedResult(hComPort,
                &ovlpRead,
                &dwBytesRead,
                TRUE		//一直阻塞直到串口读取操作完成
            );
            //----------------------------------------------------
            //输出原始数据
            //通知观察者接收数据
            if (service != nullptr) {
                service->NotifyObservers(FrameObserver::DataSource::COMPORT, read_buff, dwBytesRead);
            }
            //----------------------------------------------------
            //这里清空缓冲区可能会导致接收缓冲区数据丢失
            //PurgeComm(hComPort, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
            return dwBytesRead;
        }
    }
    return dwBytesRead;
}

/**
*   如果setQuit = true,将强迫线程退出
**/
void SerialPortListener::SetBrock(bool setQuit)
{
    isQuit = setQuit;
}

int SerialPortListener::Run()
{
    return this->Entry();
}

SerialPortListener::~SerialPortListener()
{
    if (this->hComPort != NULL) {
        ::CloseHandle(this->hComPort);
        this->hComPort = NULL;
    }
}
