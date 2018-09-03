//
// Created by Chia-Hao on 2018/8/27.
//

#ifndef XPLAY_XSHADER_H
#define XPLAY_XSHADER_H

#include <mutex>

/* Re-define enum to make independent */
enum XSHADER_TYPE
{
	XSHADER_YUV420P = 0, // Y:4, U:1, V:1
	XSHADER_NV12 = 25,   // Y:4, UV:1
	XSHADER_NV21 = 26    // Y:4, VU:1
}

class XShader {
public:
    virtual bool Init( XSHADER_TYPE type = XSHADER_YUV420P );
    virtual void Close();
    /* obtain texture and map to memory */
    virtual void GetTexture( unsigned int index, int width, int height, unsigned char* buf, bool is_alpha=false );
    virtual void Draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
    std::mutex mutex;
};


#endif //XPLAY_XSHADER_H
