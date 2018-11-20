# AndroidVideoPlayer

This project is to build a video/audio player based on FFmpeg libraray running on ARM architecture/Android System. The repository contains the project of Android Studio. Most of functions are written by C/C++ including FFmpeg demultiplexing/decoding, OpenSL audio play and OpenGL video play. Refer to [Demo](https://www.youtube.com/watch?v=8b6t6j8PdJ0])

![](https://github.com/lineagech/AndroidVideoPlayer/blob/master/BD.JPG)
`Observer is using observer design pattern that using two threads of being observed and observer, when something are updated, there is a queue to buffer things updated and then observer will consume that queue. `


'''
**FFDemux**
Constructor:  av_register_all() -> avcodec_register_all() -> avformat_network_init()
Open: avformat_open_input -> avformat_find_stream_info -> av_find_best_stream (AVMEDIA_TYPE_AUDIO/AVMEDIA_TYPE_VIDEO)
Read: AVPacket* packet = av_packet_alloc() -> av_read_frame()
'''
