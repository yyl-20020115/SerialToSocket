#ifndef TCPSERVERLISTENER_H
#define TCPSERVERLISTENER_H
#include <afxsock.h>
#include <iostream>
#include <iomanip>

#include "CommListenerBase.h"

class TCPServerListener : public CommListenerBase
{
public:
    TCPServerListener(CSocket* socket, uint8_t* recv_buff, const size_t recv_len);
    virtual ~TCPServerListener();
public:
    void SetBrock(bool setQuit);
    CSocket* GetClient();
    void WriteData(uint8_t* data, size_t len);
protected:
    int Entry();
private:
    CSocket* serverSocket;
    uint8_t* RECV_BUFF;
    const size_t BUFF_SIZE;
    volatile bool isQuit;
    CSocket* inputStream;
    CSocket* outputStream;
    CSocket* socket;

    int ReadNetPort(uint8_t* read_buff, unsigned long readLen);
};

#endif // TCPSERVERLISTENER_H
