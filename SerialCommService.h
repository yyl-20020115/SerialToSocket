#pragma once
#ifndef SERIALCOMMSERVICE_H
#define SERIALCOMMSERVICE_H

#include <set>
#include <string>
#include <sstream>

#include "CommServiceBase.h"
#include <windows.h>

class SerialCommService : public CommServiceBase
{
public:
    SerialCommService();
    virtual ~SerialCommService() override; 
public:
    //打开通信链路
    virtual void Open() override;
    //发送数据
    virtual void Send(uint8_t* data, uint32_t len) override;
    //关闭通信链路
    virtual void Close() override;
protected:

private:
    HANDLE hComPort = NULL;                    //串口句柄
    const size_t RECV_BUFF_SIZE = 10240;
    uint8_t* receive_buffer = nullptr;                //接收缓冲区
    const size_t SEND_BUFF_SIZE = 10240;
    uint8_t* send_buffer = nullptr;                //发送缓冲区
};

#endif // SERIALCOMMSERVICE_H
