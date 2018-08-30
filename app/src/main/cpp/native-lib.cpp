#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "FFDemux.h"    // for test
#include "XLog.h"
#include "IDecode.h"    // for test
#include "FFDecode.h"   // for test

#include "IVideoView.h"
#include "GLVideoView.h"
#include "FFResample.h"

class TestObs: public IObserver{
public:
    void Update(XData d)
    {
        XLOGI("TestObs Update data is %d", d.size);
    }
};

IVideoView* iview = NULL;

extern "C" JNIEXPORT jstring

JNICALL
Java_xplay_xplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    // for test //////////////////

    TestObs* tobs = new TestObs();
    IDemux* de = new FFDemux;
    //de->AddObs(tobs);
    de->Open("/sdcard/1080.mp4");

    IDecode* vdecode = new FFDecode();
    vdecode->Open(de->GetVPara());

    IDecode* adecode = new FFDecode();
    adecode->Open(de->GetAPara());

    // Send every date to vdecode and adecode
    de->AddObs(vdecode);
    de->AddObs(adecode);

    iview = new GLVideoView();
    vdecode->AddObs(iview);

    IResample* resample = new FFResample();
    resample->Open(de->GetAPara());
    adecode->AddObs(resample);

    de->Start();
    XLOGI("Demux Start!");
    vdecode->Start();
    XLOGI("Video Decoding Start!");
    adecode->Start();
    XLOGI("Audio Decoding Start!");
    ///XSleep(3000);
    //de->Stop();

    ///////////////////////////////

    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_xplay_xplay_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    ANativeWindow* window = ANativeWindow_fromSurface( env, surface );
    iview->SetRender( window );
    //XEGL::Get()->Init( window );
    //XShader shader;
    //shader.Init();

}