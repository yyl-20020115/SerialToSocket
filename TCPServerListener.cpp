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
        //std::cout << "开始监听 " << remoteAddr.IPAddress() << " : " << remoteAddr.Service() << " 网络端口!" << std::endl;
    }
    else {
        std::cerr << "无法侦听网络端口! File:" << __FILE__ << "; Line:" << __LINE__ << std::endl;
    }
}

int TCPServerListener::Entry()
{
    isQuit = false;
    NetCommService& netService = NetCommService::Instance;

    while (true)//(!TestDestroy())
    {
        /**
        *   以无阻塞的方式接收socket连接
        *   立即返回不阻塞GUI
        **/
        if (serverSocket->Accept(*socket))
        {
            Sleep(1000);             //如果没有接收到连接休眠1秒,防止CPU占满
        }
        else {

            //wxIPV4address remoteAddr;
            //socket->GetPeer(remoteAddr);
            //std::cout << "接收到客户端连接: " << remoteAddr.IPAddress() << " : " << remoteAddr.Service() << std::endl;
            ////增加计数

            netService.count++;

            //在接收完数据之前,阻塞别的操作,不阻塞GUI
            //socket->SetFlags(wxSOCKET_WAITALL|wxSOCKET_BLOCK);
            //socket->SetFlags(wxSOCKET_NOWAIT);
            //inputStream = new CSocket(*socket);
            //outputStream = new CSocket(*socket);

            while (true)//!TestDestroy())
            {
                /**
                *   避免过多的占用系统CPU
                *   这里加一点休眠释放一点时间片
                *   取值应在100ms至500ms之间,取决于每两次接收数据的间隔
                *   如果取消掉该延迟会导致CPU占用率爆满
                **/
                Sleep(200);

                if (isQuit == false)
                {
                    //------------------------------------------
                    if (true)// socket->IsData())
                    {   //有新的数据接收到
                        ReadNetPort(RECV_BUFF, BUFF_SIZE);
                    }
                    if (socket)
                    {   //检测到客户端断开连接
                        //std::cout << "检测到客户端断开连接! " << remoteAddr.IPAddress() << " : " << remoteAddr.Service() << " ......" << std::endl;
                        socket->Close();
                        //递减计数
                        if (netService.count > 0) {
                            netService.count--;
                        }
                        break;
                    }
                }
                else {
                    //当服务器要关闭时要先关掉所有客户端的连接
                    socket->Close();
                    //递减客户端连接数
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
* 向当前的输出流写入数据
**/
void TCPServerListener::WriteData(uint8_t* data, size_t len)
{
    if (outputStream != NULL) {
        outputStream->Send(data, len);
    }
}

/**
*   读取网口数据
**/
int TCPServerListener::ReadNetPort(uint8_t* read_buff, unsigned long readLen)
{
    size_t i = 0;
    inputStream->Receive(read_buff, readLen);
    size_t dwBytesRead = 0;// inputStream->LastRead();
    //----------------------------------------------------
    //输出原始数据
    if (dwBytesRead > 6)
    {
        //如果数据量大于6字节
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
        //如果数据量小于6字节
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
