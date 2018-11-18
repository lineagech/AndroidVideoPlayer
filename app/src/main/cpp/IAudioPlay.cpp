//
// Created by Chia-Hao on 2018/8/30.
//

#include "IAudioPlay.h"
#include "XLog.h"

void IAudioPlay::Update(XData data)
{
    XLOGE("IAudioPlay::Update %d", data.size);
    if( data.size < 0 || !data.data ) return;
    while( !isExit )
    {
        frames_mutex.lock();

        if (frames.size() > max_frames) {
            frames_mutex.unlock();
            XLOGE("IAudioPlay::Update max_frames");
            XSleep(1);
            continue;
        }
        frames.push_back(data);
        frames_mutex.unlock();
        break;
    }
    XLOGE("IAudioPlay::Update Exit");
}

void IAudioPlay::Clear()
{
    frames_mutex.lock();
    while( !frames.empty() )
    {
        frames.front().Drop();
        frames.pop_front();
    }
    frames_mutex.unlock();
}

XData IAudioPlay::GetData()
{
    XData d;

    isRunning = true;

    while( !isExit )
    {
        if( isPause() )
        {
            XLOGE("IAudioPlay::GetData isPause");
            XSleep(2);
            continue;
        }
        frames_mutex.lock();
        if( !frames.empty() )
        {
            d = frames.front();
            frames.pop_front();
            frames_mutex.unlock();
            pts = d.pts;
            return d;
        }
        frames_mutex.unlock();
        XSleep(1);
    }

    isRunning = false;

    return d;
}














