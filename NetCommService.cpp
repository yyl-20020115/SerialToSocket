#include "pch.h"
#include "NetCommService.h"
#include "FactoryPreset.h"
#include "TCPServerListener.h"

NetCommService NetCommService::Instance;
//串口收发缓冲区都设置为10KB大小
NetCommService::NetCommService() 
    :count()
{
    receive_buffer = new uint8_t[RECV_BUFF_SIZE];
    send_buffer = new uint8_t[SEND_BUFF_SIZE];
    is_running = false;
}

/**
*   开启网络端口
**/
void NetCommService::Open()
{
    if (this->is_running)
    {
        Close();
    }

    //从配置文件中读取网络通信参数
    auto& preset = FactoryPreset::GetInstance();

    this->socket_server = new CSocket();
    if (!socket_server->Create(preset.port,SOCK_STREAM,preset.host.c_str()))
    {
        //unable to start server
    }
    else {
        this->listener = new TCPServerListener(socket_server, receive_buffer, RECV_BUFF_SIZE);
        this->is_running = true;
    }
}

/**
*   通过网络发送数据
**/
void NetCommService::Send(uint8_t* data, uint32_t len)
{
    if (listener != nullptr) {
        TCPServerListener* tcpListener = dynamic_cast<TCPServerListener *>(listener);
        if (tcpListener != nullptr) {
            tcpListener->WriteData(data, len);
        }
    }
}

/**
*   关闭网络端口
**/
void NetCommService::Close()
{
    if (this->listener != nullptr)
    {
        this->listener->SetBrock(true);
        this->listener = nullptr;
    }
    if (this->socket_server != nullptr)
    {
        this->socket_server->Close();
        this->socket_server = nullptr;
    }
}

NetCommService::~NetCommService()
{
    if (this->receive_buffer != nullptr) {
        delete[] this->receive_buffer;
        this->receive_buffer = nullptr;
    }
    if (this->send_buffer != nullptr) {
        delete[] send_buffer;
        this->send_buffer = nullptr;
    }
    if (this->socket_server != nullptr) {
        delete this->socket_server;
        this->socket_server = nullptr;
    }
}
