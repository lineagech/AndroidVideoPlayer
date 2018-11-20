# AndroidVideoPlayer

This project is to build a video/audio player based on FFmpeg libraray running on ARM architecture/Android System. The repository contains the project of Android Studio. Most of functions are written by C/C++ including FFmpeg demultiplexing/decoding, OpenSL audio play and OpenGL video play. Refer to [Demo](https://www.youtube.com/watch?v=8b6t6j8PdJ0])

![](https://github.com/lineagech/AndroidVideoPlayer/blob/master/BD.JPG)
`Observer is using observer design pattern that using two threads of being observed and observer, when something are updated, there is a queue to buffer things updated and then observer will consume that queue. `


**FFDemux**
```
Constructor:  av_register_all() -> avcodec_register_all() -> avformat_network_init()
Open: avformat_open_input -> avformat_find_stream_info -> av_find_best_stream (AVMEDIA_TYPE_AUDIO/AVMEDIA_TYPE_VIDEO)
Read: AVPacket* packet = av_packet_alloc() -> av_read_frame()
```

**FFDecode**
```
Open: AVCodec* codec = avcodec_find_decoder( p_avcodec_para->codec_id ) -> avcodec_alloc_context3 -> avcodec_parameters_to_context -> avcodec_open2
SendPacket: avcodec_send_packet( AVCodecContext* , AVPacket* )
RecvFrame: avcodec_receive_frame( AVCodecContext* , AVFrame* )
```

**FFResample**
```
Open: SwrContext* = swr_alloc() -> swr_alloc_set_opts() (AV_SAMPLE_FMT_S16)
Resample: swr_convert( SwrContext* , uint8_t **, int , const uint8_t **, int )
```

**SLAudioPlay**
```
CreateSL: slCreateEngine -> Realize(SLEngineItf) -> GetInterface(SL_IID_ENGINE)
StartPlay: CreateOutputMix -> Realize(Mixer) -> SLDataLocator_OutputMix, SLDataSink, SLDataLocator_AndroidSimpleBufferQueue, SLDataFormat_PCM, SLDataSource -> CreateAudioPlayer() -> Realize(Player) -> GetInterface(SL_IID_PLAY) -> GetInterface(SL_IID_BUFFERQUEUE) -> RegisterCallback -> SetPlayState(SL_PLAYSTATE_PLAYING) -> Enqueue
PcmCall: call PlayCall() -> Enqueue
```

**GLVideoView**
```
Render: XTexture::Create() -> Init XTexture -> Draw 
```

