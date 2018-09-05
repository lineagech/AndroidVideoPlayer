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
    virtual bool Start();
    // Safe stop
    virtual void Stop();

    virtual void Pause( bool pause_or_not );
    virtual bool isPause();

    // Main entry function
    virtual void Main() {}

private:
    void ThreadMain();

protected:
    bool isExit = false;
    bool isRunning = false;
    bool isPaused = false;
    bool isPausing = false;
};


#endif //XPLAY_XTHREAD_H
