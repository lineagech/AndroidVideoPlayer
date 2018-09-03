//
// Created by Chia-Hao on 2018/8/27.
//

#ifndef XPLAY_XTEXTURE_H
#define XPLAY_XTEXTURE_H

enum XTEXTURE_TYPE
{
	XTEXTURE_YUV420P = 0, // Y:4, U:1, V:1
	XTEXTURE_NV12 = 25,   // Y:4, UV:1
	XTEXTURE_NV21 = 26    // Y:4, VU:1
}

class XTexture {
public:
    static XTexture* Create();
    virtual bool Init(void* win, XTEXTURE_TYPE type=XTEXTURE_YUV420P) = 0;
    virtual void Draw(unsigned char* data[], int width, int height) = 0;
    virtual void Drop() = 0;
    virtual ~XTexture(){};
protected:
	XTexture(){};
};


#endif //XPLAY_XTEXTURE_H
