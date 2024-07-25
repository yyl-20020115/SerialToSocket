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
    //��ͨ����·
    virtual void Open() override;
    //��������
    virtual void Send(uint8_t* data, uint32_t len) override;
    //�ر�ͨ����·
    virtual void Close() override;
protected:

private:
    HANDLE hComPort = NULL;                    //���ھ��
    const size_t RECV_BUFF_SIZE = 10240;
    uint8_t* receive_buffer = nullptr;                //���ջ�����
    const size_t SEND_BUFF_SIZE = 10240;
    uint8_t* send_buffer = nullptr;                //���ͻ�����
};

#endif // SERIALCOMMSERVICE_H
