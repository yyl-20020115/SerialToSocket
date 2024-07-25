#include "pch.h"
#include "SerialCommService.h"
#include "SerialPortListener.h"
#include "FactoryPreset.h"

//�����շ�������������Ϊ10KB��С
SerialCommService::SerialCommService() 
    : RECV_BUFF_SIZE(10240)
    , SEND_BUFF_SIZE(10240)
{
    this->receive_buffer = new uint8_t[RECV_BUFF_SIZE];
    this->send_buffer = new uint8_t[SEND_BUFF_SIZE];
    this->is_running = false;
}

/**
*   �򿪴���
**/
void SerialCommService::Open()
{
    if (is_running)
    {
        Close();
    }
    //�������ļ��ж�ȡ����
    auto& preset= FactoryPreset::GetInstance();

    //���ﴮ�ں���Ҫ��ǰ׺
    this->hComPort = CreateFile((L"\\\\.\\"+preset.com).c_str(),                         //ͨ�ſ�
        GENERIC_READ | GENERIC_WRITE,      //�����д
        0,                               //��ռ��ʽ
        NULL,                            //��ȫ����
        OPEN_EXISTING,                   //
        FILE_FLAG_OVERLAPPED,            //�ص�IO,�첽����
        NULL);                           //��ʹ��ģ��

    if (hComPort != INVALID_HANDLE_VALUE)
    {
        DCB comPortDCB = { 0 };

        //�������������������С
        SetupComm(hComPort, RECV_BUFF_SIZE, SEND_BUFF_SIZE);

        COMMTIMEOUTS commTimeout={0};
        //���ö�ȡ��ʱ
        commTimeout.ReadIntervalTimeout = 100;
        commTimeout.ReadTotalTimeoutMultiplier = 100;		//��ȡ��д��������ֶܷ�ν���,ÿ�εļ����ʱ
        commTimeout.ReadTotalTimeoutConstant = 200;	        //�������ܵĶ�ȡ��ʱ,����������ܴ�,Ӧ�Ӵ����ֵ
        //����д�볬ʱ
        commTimeout.WriteTotalTimeoutMultiplier = 100;
        commTimeout.WriteTotalTimeoutConstant = 100;
        SetCommTimeouts(hComPort, &commTimeout);

        //��ȡ��֮ǰ�Ĵ��ڲ���
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
        //�޸Ĵ��ڲ���
        SetCommState(hComPort, &comPortDCB);
        //��շ��ͺͽ��ջ�����
        PurgeComm(hComPort, PURGE_TXCLEAR | PURGE_RXCLEAR);
        //------------------------------
        //�������ڼ����߳�
        this->listener = new SerialPortListener(this, hComPort, receive_buffer, RECV_BUFF_SIZE);

        //��Ƿ���Ϊ����̬
        is_running = true;
    }
}

/**
*   ͨ�����ڷ�������
**/
void SerialCommService::Send(uint8_t* data, uint32_t len)
{
    BOOL bWriteRet = FALSE;
    DWORD dwWritedNum = 0;
    OVERLAPPED ovlpWrite = { 0 };
    ovlpWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    bWriteRet = WriteFile(hComPort, data, len, &dwWritedNum, &ovlpWrite);
    if (!bWriteRet)			//���첽������,ReadFile����FALSE��һ�����Ƕ�ȡ����
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            //����һ��ʱ��ֱ�����ڶ�ȡ�������
            WaitForSingleObject(ovlpWrite.hEvent, 1000);
        }
    }
}

/**
*   �رյ�ǰ����
**/
void SerialCommService::Close()
{
    //��ȷ��Listener�Ѿ��ر�
    if (listener != nullptr)
    {
        this->listener->SetBrock(true);
        ::WaitForSingleObject(this->listener->m_hThread, INFINITY);
        delete this->listener;
        this->listener = nullptr;
    }
    //�رմ���
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
