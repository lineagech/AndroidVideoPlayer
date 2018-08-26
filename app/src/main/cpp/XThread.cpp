//
// Created by Chia-Hao on 2018/8/26.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>

using namespace std;

// Start thread
void XThread::Start()
{
    thread thread_(&XThread::ThreadMain, this);
    thread_.detach();
}

void XThread::ThreadMain();
{
    XLOGI("Entering Thread Function");
    Main();
    XLOGI("Exiting Thread Function");
}

// Safe stop
void XThread::Stop()
{

}