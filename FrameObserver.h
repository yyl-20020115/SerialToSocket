#pragma once
#ifndef FRAMEOBSERVER_H
#define FRAMEOBSERVER_H

#include <stdint.h>

class FrameObserver
{
public:
    /**
    * 定义数据的来源
    **/
    enum class DataSource
    {
        COMPORT,    //串口
        NETWORK     //网络
    } ;

    //定义接收数据的事件
    virtual void OnReciveData(DataSource source, uint8_t* data, size_t len) = 0;

};

#endif // FRAMEOBSERVER_H
