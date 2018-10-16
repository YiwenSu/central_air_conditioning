#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QThread>
#include "ac.h"
class WorkThread : public QThread
{
public:
    WorkThread();
    void set(AC *a);
protected:
    void run();
private:
    AC *theac;
};

#endif // WORKTHREAD_H
