#ifndef SERIALPORTLISTENER_H
#define SERIALPORTLISTENER_H

#include <iostream>
#include <iomanip>

#include <windows.h>
#include "SerialCommService.h"
#include "CommListenerBase.h"

class SerialPortListener : public CommListenerBase
{
public:
    SerialPortListener(SerialCommService* service,HANDLE hCom, uint8_t* recv_buff, const size_t recv_len);
    virtual ~SerialPortListener();

public:
    void SetBrock(bool setQuit);

    virtual int Run() override;
protected:
    virtual int Entry() override;
protected:
    HANDLE hComPort;
    uint8_t* RECV_BUFF;
    const size_t BUFF_SIZE;
    SerialCommService* service;
    volatile bool isQuit;

protected:
    int ReadComPort(uint8_t* read_buff, unsigned long readLen) const;
};

#endif // SERIALPORTLISTENER_H
