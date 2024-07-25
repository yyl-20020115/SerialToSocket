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
*   ������̺߳���
**/
int SerialPortListener::Entry()
{
    isQuit = false;
    while (::WaitForSingleObject(this->hComPort,!10) && !isQuit)
    {
        /**
        *   ��������ռ��ϵͳCPU
        *   �����һ�������ͷ�һ��ʱ��Ƭ
        *   ȡֵӦ��10ms��100ms֮��,ȡ����ÿ���ν������ݵļ��
        *   ���ȡ�������ӳٻᵼ��CPUռ���ʱ���
        **/

        //��ʼ��ȡ��������
        if (!isQuit)
        {
            //��ջ����������¶�ȡ
            memset(RECV_BUFF, 0, BUFF_SIZE);
            ReadComPort(RECV_BUFF, BUFF_SIZE);
        }
    }
    Delete();

    return 0;
}

/**
*   �Ӵ��ڶ�ȡ����
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
        return -1;			//��ȡ��������û������
    }
    memset(&ovlpRead, 0, sizeof(OVERLAPPED));
    ovlpRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    bReadRet = ReadFile(hComPort, read_buff, readLen, &dwBytesRead, &ovlpRead);
    if (!bReadRet)			//���첽������,ReadFile����FALSE��һ�����Ƕ�ȡ����
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            GetOverlappedResult(hComPort,
                &ovlpRead,
                &dwBytesRead,
                TRUE		//һֱ����ֱ�����ڶ�ȡ�������
            );
            //----------------------------------------------------
            //���ԭʼ����
            //֪ͨ�۲��߽�������
            if (service != nullptr) {
                service->NotifyObservers(FrameObserver::DataSource::COMPORT, read_buff, dwBytesRead);
            }
            //----------------------------------------------------
            //������ջ��������ܻᵼ�½��ջ��������ݶ�ʧ
            //PurgeComm(hComPort, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
            return dwBytesRead;
        }
    }
    return dwBytesRead;
}

/**
*   ���setQuit = true,��ǿ���߳��˳�
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
