#include "pch.h"
#include "CommServiceBase.h"

CommServiceBase::CommServiceBase()
    :observers()
{
}

void CommServiceBase::AddObserver(FrameObserver* observer)
{
    for (auto itert = observers.begin(); itert != observers.end();)
    {
        if (observer == *itert)
        {
            itert = observers.erase(itert);
        }
        else {
            itert++;
        }
    }
    observers.push_back(observer);
}

/**
* 通知所有观察者
**/
void CommServiceBase::NotifyObservers(FrameObserver::DataSource source, uint8_t* data, size_t len)
{
    for (auto o : observers)
    {
        o->OnReciveData(source, data, len);
    }
}

CommListenerBase* CommServiceBase::GetCommListener()
{
    return listener;
}

/**
*   通信链路是否已经打开
**/
bool CommServiceBase::IsOpen() const
{
    return is_running;
}

CommServiceBase::~CommServiceBase()
{
    if (this->listener != nullptr) {
        delete this->listener;
        this->listener = nullptr;
    }
    this->observers.clear();
}
