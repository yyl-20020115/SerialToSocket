#include "pch.h"
#include "SerialCommService.h"
#include "SerialPortListener.h"
#include "FactoryPreset.h"

//网络收发缓冲区都设置为10KB大小
SerialCommService::SerialCommService() 
    : RECV_BUFF_SIZE(10240)
    , SEND_BUFF_SIZE(10240)
{
    this->receive_buffer = new uint8_t[RECV_BUFF_SIZE];
    this->send_buffer = new uint8_t[SEND_BUFF_SIZE];
    this->is_running = false;
}

/**
*   打开串口
**/
void SerialCommService::Open()
{
    if (is_running)
    {
        Close();
    }
    //从配置文件中读取串口
    auto& preset= FactoryPreset::GetInstance();

    //这里串口号需要加前缀
    this->hComPort = CreateFile((L"\\\\.\\"+preset.com).c_str(),                         //通信口
        GENERIC_READ | GENERIC_WRITE,      //允许读写
        0,                               //独占方式
        NULL,                            //安全属性
        OPEN_EXISTING,                   //
        FILE_FLAG_OVERLAPPED,            //重叠IO,异步串口
        NULL);                           //不使用模板

    if (hComPort != INVALID_HANDLE_VALUE)
    {
        DCB comPortDCB = { 0 };

        //设置输入输出缓冲区大小
        SetupComm(hComPort, RECV_BUFF_SIZE, SEND_BUFF_SIZE);

        COMMTIMEOUTS commTimeout={0};
        //设置读取超时
        commTimeout.ReadIntervalTimeout = 100;
        commTimeout.ReadTotalTimeoutMultiplier = 100;		//读取和写入操作可能分多次进行,每次的间隔超时
        commTimeout.ReadTotalTimeoutConstant = 200;	        //这里是总的读取超时,如果数据量很大,应加大该数值
        //设置写入超时
        commTimeout.WriteTotalTimeoutMultiplier = 100;
        commTimeout.WriteTotalTimeoutConstant = 100;
        SetCommTimeouts(hComPort, &commTimeout);

        //获取到之前的串口参数
        GetCommState(hComPort, &comPortDCB);

        comPortDCB.BaudRate = (DWORD)preset.baudrate;
        comPortDCB.ByteSize = (BYTE)preset.databits;
        if (preset.parity == L"None") {
            comPortDCB.Parity = NOPARITY;
        }
        else if (preset.parity == L"Odd") {
            comPortDCB.Parity = ODDPARITY;
        }
        else if (preset.parity == L"Even") {
            comPortDCB.Parity = EVENPARITY;
        }
        else if (preset.parity == L"Mark") {
            comPortDCB.Parity = MARKPARITY;
        }
        else if (preset.parity == L"Space") {
            comPortDCB.Parity = SPACEPARITY;
        }

        if (preset.stopbits == L"1") {
            comPortDCB.StopBits = ONESTOPBIT;
        }
        else if (preset.stopbits == L"1.5") {
            comPortDCB.StopBits = ONE5STOPBITS;
        }
        else if (preset.stopbits == L"2") {
            comPortDCB.StopBits = TWOSTOPBITS;
        }
        //修改串口参数
        SetCommState(hComPort, &comPortDCB);
        //清空发送和接收缓冲区
        PurgeComm(hComPort, PURGE_TXCLEAR | PURGE_RXCLEAR);
        //------------------------------
        //创建串口监听线程
        this->listener = new SerialPortListener(this, hComPort, receive_buffer, RECV_BUFF_SIZE);

        //标记服务为运行态
        is_running = true;
    }
}

/**
*   通过串口发送数据
**/
void SerialCommService::Send(uint8_t* data, uint32_t len)
{
    BOOL bWriteRet = FALSE;
    DWORD dwWritedNum = 0;
    OVERLAPPED ovlpWrite = { 0 };
    ovlpWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    bWriteRet = WriteFile(hComPort, data, len, &dwWritedNum, &ovlpWrite);
    if (!bWriteRet)			//在异步串口中,ReadFile返回FALSE不一定就是读取错误
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            //阻塞一段时间直到串口读取操作完成
            WaitForSingleObject(ovlpWrite.hEvent, 1000);
        }
    }
}

/**
*   关闭当前串口
**/
void SerialCommService::Close()
{
    //先确保Listener已经关闭
    if (listener != nullptr)
    {
        this->listener->SetBrock(true);
        ::WaitForSingleObject(this->listener->m_hThread, INFINITY);
        delete this->listener;
        this->listener = nullptr;
    }
    //关闭串口
    if (this->hComPort != NULL) {
        CloseHandle(hComPort);
        this->hComPort = NULL;
    }
    this->is_running = false;
}

SerialCommService::~SerialCommService()
{
    if (this->receive_buffer != nullptr) {
        delete[] this->receive_buffer;
        this->receive_buffer = nullptr;
    }
    if (this->send_buffer != nullptr) {
        delete[] send_buffer;
        this->send_buffer = nullptr;
    }
}
