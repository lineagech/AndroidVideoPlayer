//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_XDATA_H
#define XPLAY_XDATA_H


class XData {
public:
    unsigned char* data = 0;
    unsigned char* datas[8] = {0};
    int width = 0;
    int height = 0;
    int size = 0;
    bool isAudio = false;
    void Drop();
};


#endif //XPLAY_XDATA_H
