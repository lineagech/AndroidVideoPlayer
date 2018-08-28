//
// Created by Chia-Hao on 2018/8/27.
//

#ifndef XPLAY_XEGL_H
#define XPLAY_XEGL_H


class XEGL {
public:
    virtual bool Init( void* win ) = 0;
    static XEGL* Get();
    virtual void Draw() = 0;
protected:
    XEGL(){}
};


#endif //XPLAY_XEGL_H
