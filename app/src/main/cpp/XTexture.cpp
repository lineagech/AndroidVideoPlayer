//
// Created by Chia-Hao on 2018/8/27.
//

#include "XTexture.h"
#include "XLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture:public XTexture
{
public:
    XShader shader;
    virtual bool Init( void* win )
    {
        if( !win ) {
            XLOGE("XTexture Failed.");
            return false;
        }
        if( !XEGL::Get()->Init(win) ) return false;
        shader.Init();
        XLOGE("XTexture Init Succeeded.");
        return true;
    }
    virtual void Draw(unsigned char* data[], int width, int height)
    {
        shader.GetTexture(0, width, height, data[0]);
        shader.GetTexture(1, width/2, height/2, data[1]);
        shader.GetTexture(2, width/2, height/2, data[2]);

        shader.Draw();
        XEGL::Get()->Draw();
    }
};

XTexture* XTexture::Create()
{
    return new CXTexture();
}