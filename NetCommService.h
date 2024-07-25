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
    //��ͨ����·
    virtual void Open() override;
    //��������
    virtual void Send(uint8_t* data, uint32_t len) override;
    //�ر�ͨ����·
    virtual void Close() override;

public:
    std::atomic<int> count;

protected:
    //ͳ�ƿͻ���������

    uint8_t* receive_buffer = nullptr;                //���ջ�����
    uint8_t* send_buffer = nullptr;                //���ͻ�����
    CSocket* socket_server = nullptr;       //��������Socket

};

#endif // NETCOMMSERVICE_H
