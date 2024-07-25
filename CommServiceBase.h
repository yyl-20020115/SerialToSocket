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
    //��ͨ����·
    virtual void Open() = 0;
    //��������
    virtual void Send(uint8_t* data, uint32_t size) = 0;
    //�ر�ͨ����·
    virtual void Close() = 0;
    //���Э��֡�۲���
    void AddObserver(FrameObserver* observer);
    //��ȡ��ǰͨ����·�ļ����߳�
    CommListenerBase* GetCommListener();
    //֪ͨ���й۲���
    void NotifyObservers(FrameObserver::DataSource source, uint8_t* data, size_t len);
    //ͨ����·�Ƿ��Ѿ���
    bool IsOpen() const;
protected:
    //��ʶͨ�ŷ����Ƿ�������
    bool is_running = false;

    /**
    *   ÿ��ͨ��ģʽʹ���Լ��ķ�ʽ��������
    **/
    CommListenerBase* listener = nullptr;
private:
    std::vector<FrameObserver*> observers;
};

#endif // COMMSERVICEBASE_H
