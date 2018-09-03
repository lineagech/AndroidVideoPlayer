//
// Created by Chia-Hao on 2018/8/30.
//

#ifndef XPLAY_IAUDIOPLAY_H
#define XPLAY_IAUDIOPLAY_H


#include <list>
#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay : public IObserver {
public:
    /* blocking mode, will being blocked when frames size is equal to max_frames */
    virtual void Update(XData data);
    /* get queued data, will block if no data exists */
    virtual XData GetData();
    virtual bool StartPlay(XParameter out) = 0;
    virtual void Close() = 0;
    int max_frames = 100;
    int pts = 0;
protected:
    std::list<XData> frames;
    std::mutex frames_mutex;
};


#endif //XPLAY_IAUDIOPLAY_H
