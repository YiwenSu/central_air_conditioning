#include "workthread.h"

WorkThread::WorkThread()
{

}

void WorkThread::run()
{
    while(true)
    {
        this->theac->schedule(this->theac->getMode());
    }
}

void WorkThread::set(AC *a)
{
    this->theac = a;
}
