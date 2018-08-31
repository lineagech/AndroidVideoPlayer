//
// Created by Chia-Hao on 2018/8/27.
//

#include "GLVideoView.h"
#include "XTexture.h"
#include "XLog.h"

void GLVideoView::SetRender(void* win)
{
    view = win;
    XLOGE("Set Render!");
}

void GLVideoView::Render(XData data)
{
    if( !view ) return;
    if( !text )
    {
        text = XTexture::Create();
        text->Init(view, (XTEXTURE_TYPE)data.format);
    }
    XLOGE("Render Drawing!");
    text->Draw( data.datas, data.width, data.height );

}