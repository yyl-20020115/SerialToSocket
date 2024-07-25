#pragma once
#ifndef FRAMEOBSERVER_H
#define FRAMEOBSERVER_H

#include <stdint.h>

class FrameObserver
{
public:
    /**
    * �������ݵ���Դ
    **/
    enum class DataSource
    {
        COMPORT,    //����
        NETWORK     //����
    } ;

    //����������ݵ��¼�
    virtual void OnReciveData(DataSource source, uint8_t* data, size_t len) = 0;

};

#endif // FRAMEOBSERVER_H
