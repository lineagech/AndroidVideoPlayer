//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_XPARAMETER_H
#define XPLAY_XPARAMETER_H


struct AVCodecParameters; // direct declaration, dont care its real implementation and include its header
class XParameter {
public:
    AVCodecParameters* para;
    int channels = 2;
    int sample_rate = 44100;
};


#endif //XPLAY_XPARAMETER_H
