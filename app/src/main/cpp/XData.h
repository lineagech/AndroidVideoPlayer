//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_XDATA_H
#define XPLAY_XDATA_H

enum XDataType{
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1
};

class XData {
public:
    int type = 0;
    int pts = 0;
    unsigned char* data = 0;
    unsigned char* datas[8] = {0};
    int width = 0;
    int height = 0;
    int size = 0;
    int format = 0;
    bool isAudio = false;
    void Drop();
    bool Alloc(int size, const char* data=0);
};


#endif //XPLAY_XDATA_H
