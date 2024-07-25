#ifndef COMMLISTENERBASE_H
#define COMMLISTENERBASE_H

#include "framework.h"

class CommListenerBase : public CWinThread
{
public:
    CommListenerBase() {}
    virtual ~CommListenerBase() {}
public:
    virtual int Entry() = 0;
    virtual void SetBrock(bool setQuit) = 0;

protected:
};

#endif // COMMLISTENERBASE_H
