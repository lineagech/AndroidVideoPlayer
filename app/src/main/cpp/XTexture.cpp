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
    XTEXTURE_TYPE type;
    std::mutex mutex;

    virtual void Drop()
    {
        mutex.lock();
        XEGL::Get()->Close();
        shader.Close();
        mutex.unlock();
        delete this;
    }

    virtual bool Init( void* win, XTEXTURE_TYPE type )
    {       
        mutex.lock();

        XEGL::Get()->Close();
        shader.Close();

        this->type = type;
        if( !win ) {
            mutex.unlock();
            XLOGE("XTexture Failed.");
            return false;
        }
        if( !XEGL::Get()->Init(win) ) {
            mutex.unlock();
            return false;
        }
        shader.Init((XSHADER_TYPE)type);
        mutex.unlock();
        XLOGE("XTexture Init Succeeded.");
        return true;
    }
    virtual void Draw(unsigned char* data[], int width, int height)
    {   
        mutex.lock();
        shader.GetTexture(0, width, height, data[0]);
        if( type == XTEXTURE_YUV420P )
        {
            shader.GetTexture(1, width/2, height/2, data[1]); // U
            shader.GetTexture(2, width/2, height/2, data[2]); // V
        }
        else {
            shader.GetTexture(1, width/2, height/2, data[1], true); // UV
        }
        shader.Draw();
        XEGL::Get()->Draw();
        mutex.unlock();
    }
};

XTexture* XTexture::Create()
{
    return new CXTexture();
}