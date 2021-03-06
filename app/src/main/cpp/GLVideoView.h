//
// Created by Chia-Hao on 2018/8/27.
//

#ifndef XPLAY_GLVIDEOVIEW_H
#define XPLAY_GLVIDEOVIEW_H


#include "XData.h"
#include "IVideoView.h"
#include <mutex>

class XTexture;

class GLVideoView: public IVideoView {
public:
    virtual void SetRender(void* win);
    virtual void Render(XData data);
    virtual void Close();

protected:
    void* view = 0;
    XTexture* text = 0;
    std::mutex mutex;
};


#endif //XPLAY_GLVIDEOVIEW_H
