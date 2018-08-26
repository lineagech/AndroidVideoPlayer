//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_XTHREAD_H
#define XPLAY_XTHREAD_H

void XSleep(int ms);
/* Use C++11 Thread Library */
class XThread {
public:
    // Start thread
    virtual void Start();
    // Safe stop
    virtual void Stop();

    // Main entry function
    virtual void Main() {}

private:
    void ThreadMain();

protected:
    bool isExit = false;
    bool isRunning = false;
};


#endif //XPLAY_XTHREAD_H