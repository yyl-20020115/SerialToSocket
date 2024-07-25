#ifndef COMMSERVICEBASE_H
#define COMMSERVICEBASE_H
#include "CommListenerBase.h"
#include "FrameObserver.h"
#include <vector>

class CommServiceBase
{
public:
    CommServiceBase();
    virtual ~CommServiceBase();
public:
    //打开通信链路
    virtual void Open() = 0;
    //发送数据
    virtual void Send(uint8_t* data, uint32_t size) = 0;
    //关闭通信链路
    virtual void Close() = 0;
    //添加协议帧观察者
    void AddObserver(FrameObserver* observer);
    //获取当前通信链路的监听线程
    CommListenerBase* GetCommListener();
    //通知所有观察者
    void NotifyObservers(FrameObserver::DataSource source, uint8_t* data, size_t len);
    //通信链路是否已经打开
    bool IsOpen() const;
protected:
    //标识通信服务是否还在运行
    bool is_running = false;

    /**
    *   每种通信模式使用自己的方式接收数据
    **/
    CommListenerBase* listener = nullptr;
private:
    std::vector<FrameObserver*> observers;
};

#endif // COMMSERVICEBASE_H
