#include "pch.h"
#include "TCPServerListener.h"
#include "NetCommService.h"

TCPServerListener::TCPServerListener(CSocket* socket, uint8_t* recv_buff, const size_t recv_len) :
    serverSocket(socket),
    RECV_BUFF(recv_buff),
    BUFF_SIZE(recv_len),
    isQuit(false)
{
    if (true)// (wxThread::Run() == wxTHREAD_NO_ERROR)
    {
        //wxIPV4address remoteAddr;
        //serverSocket->GetLocal(remoteAddr);
        //std::cout << "��ʼ���� " << remoteAddr.IPAddress() << " : " << remoteAddr.Service() << " ����˿�!" << std::endl;
    }
    else {
        std::cerr << "�޷���������˿�! File:" << __FILE__ << "; Line:" << __LINE__ << std::endl;
    }
}

int TCPServerListener::Entry()
{
    isQuit = false;
    NetCommService& netService = NetCommService::Instance;

    while (true)//(!TestDestroy())
    {
        /**
        *   ���������ķ�ʽ����socket����
        *   �������ز�����GUI
        **/
        if (serverSocket->Accept(*socket))
        {
            Sleep(1000);             //���û�н��յ���������1��,��ֹCPUռ��
        }
        else {

            //wxIPV4address remoteAddr;
            //socket->GetPeer(remoteAddr);
            //std::cout << "���յ��ͻ�������: " << remoteAddr.IPAddress() << " : " << remoteAddr.Service() << std::endl;
            ////���Ӽ���

            netService.count++;

            //�ڽ���������֮ǰ,������Ĳ���,������GUI
            //socket->SetFlags(wxSOCKET_WAITALL|wxSOCKET_BLOCK);
            //socket->SetFlags(wxSOCKET_NOWAIT);
            //inputStream = new CSocket(*socket);
            //outputStream = new CSocket(*socket);

            while (true)//!TestDestroy())
            {
                /**
                *   ��������ռ��ϵͳCPU
                *   �����һ�������ͷ�һ��ʱ��Ƭ
                *   ȡֵӦ��100ms��500ms֮��,ȡ����ÿ���ν������ݵļ��
                *   ���ȡ�������ӳٻᵼ��CPUռ���ʱ���
                **/
                Sleep(200);

                if (isQuit == false)
                {
                    //------------------------------------------
                    if (true)// socket->IsData())
                    {   //���µ����ݽ��յ�
                        ReadNetPort(RECV_BUFF, BUFF_SIZE);
                    }
                    if (socket)
                    {   //��⵽�ͻ��˶Ͽ�����
                        //std::cout << "��⵽�ͻ��˶Ͽ�����! " << remoteAddr.IPAddress() << " : " << remoteAddr.Service() << " ......" << std::endl;
                        socket->Close();
                        //�ݼ�����
                        if (netService.count > 0) {
                            netService.count--;
                        }
                        break;
                    }
                }
                else {
                    //��������Ҫ�ر�ʱҪ�ȹص����пͻ��˵�����
                    socket->Close();
                    //�ݼ��ͻ���������
                    if (netService.count > 0) {
                        netService.count--;
                    }
                    isQuit = true;
                    Delete();
                    break;
                }
            }
        }
    }
    return 0;
}

/**
* ��ǰ�������д������
**/
void TCPServerListener::WriteData(uint8_t* data, size_t len)
{
    if (outputStream != NULL) {
        outputStream->Send(data, len);
    }
}

/**
*   ��ȡ��������
**/
int TCPServerListener::ReadNetPort(uint8_t* read_buff, unsigned long readLen)
{
    size_t i = 0;
    inputStream->Receive(read_buff, readLen);
    size_t dwBytesRead = 0;// inputStream->LastRead();
    //----------------------------------------------------
    //���ԭʼ����
    if (dwBytesRead > 6)
    {
        //�������������6�ֽ�
        for (i = 0; i < 3; i++)
        {
            std::cout << "0x" << std::setiosflags(std::ios::uppercase) << std::hex << std::setfill('0') << std::setw(2) << (int)read_buff[i] << " ";
        }
        std::cout << "... ";
        for (i = (dwBytesRead - 3); i < dwBytesRead; i++)
        {
            std::cout << "0x" << std::setiosflags(std::ios::uppercase) << std::hex << std::setfill('0') << std::setw(2) << (int)read_buff[i] << " ";
        }
        std::cout << std::dec << std::endl;
    }
    else {
        //���������С��6�ֽ�
        for (i = 0; i < dwBytesRead; i++)
        {
            std::cout << "0x" << std::setiosflags(std::ios::uppercase) << std::hex << std::setfill('0') << std::setw(2) << (int)read_buff[i] << " ";
        }
        std::cout << std::dec << std::endl;
    }
    //----------------------------
    NetCommService& netService = NetCommService::Instance;
    netService.NotifyObservers(FrameObserver::DataSource::NETWORK, read_buff, dwBytesRead);

    return dwBytesRead;
}

CSocket* TCPServerListener::GetClient()
{
    return socket;
}

void TCPServerListener::SetBrock(bool setQuit)
{
    isQuit = setQuit;
}

TCPServerListener::~TCPServerListener()
{
    if (socket != nullptr)
    {
        delete this->socket;
        this->socket = nullptr;
    }
}
