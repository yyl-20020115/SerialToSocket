#ifndef NETCOMMSERVICE_H
#define NETCOMMSERVICE_H
#include <atomic>
#include <afxsock.h>
#include "CommServiceBase.h"

class NetCommService : public CommServiceBase
{
public:
    static NetCommService Instance;
public:
    const size_t RECV_BUFF_SIZE = 10240;
    const size_t SEND_BUFF_SIZE = 10240;
public:
    NetCommService();
    virtual ~NetCommService() override;
public:
    //打开通信链路
    virtual void Open() override;
    //发送数据
    virtual void Send(uint8_t* data, uint32_t len) override;
    //关闭通信链路
    virtual void Close() override;

public:
    std::atomic<int> count;

protected:
    //统计客户端连接数

    uint8_t* receive_buffer = nullptr;                //接收缓冲区
    uint8_t* send_buffer = nullptr;                //发送缓冲区
    CSocket* socket_server = nullptr;       //服务器端Socket

};

#endif // NETCOMMSERVICE_H
