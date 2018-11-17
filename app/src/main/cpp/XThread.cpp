//
// Created by Chia-Hao on 2018/8/26.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>

using namespace std;

void XSleep(int ms)
{
    chrono::microseconds duraion(ms);
    this_thread::sleep_for(duraion);
}

// Start thread
bool XThread::Start()
{
    isExit = false;
    isPaused = false;
    thread thread_(&XThread::ThreadMain, this);
    thread_.detach();
    return true;
}

void XThread::ThreadMain()
{
    isRunning = true;
    XLOGI("Entering Thread Function");
    this->Main();
    XLOGI("Exiting Thread Function");
    isRunning = false;
}

// Safe stop
void XThread::Stop()
{
    isExit = true;
    for( int i = 0; i < 200; i++ )
    {
        if( !isRunning )
        {
            XLOGI("Stopping Thread Succeed");
            return;
        }
        XSleep(1);
    }
    XLOGI("Stopping Thread Overtime !!!");
}

#define SleepCount (10)

void XThread::Pause( bool pause_or_not )
{
    isPaused = pause_or_not;

    /* Each thread should query isPause() first before continuing running i*/
    for( int count = 0; count < SleepCount; count++ )
    {
        if( isPaused == isPausing ) break;
        XSleep( 10 );
    }
}

bool XThread::isPause()
{
    isPausing = isPaused;
    return isPaused;
}