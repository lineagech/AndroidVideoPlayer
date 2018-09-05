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
#include "IAudioPlay.h"
#include "SLAudioPlay.h"
#include "IPlayer.h"

#include "FFPlayerBuilder.h"
#include "IPlayerProxy.h"

class TestObs: public IObserver{
public:
    void Update(XData d)
    {
        XLOGI("TestObs Update data is %d", d.size);
    }
};

IVideoView* iview = NULL;
static IPlayer* player = NULL;

extern "C" jint JNI_OnLoad(JavaVM* vm, void* res)
{
    //FFDecode::InitHard(vm);
    IPlayerProxy::Get()->Init(vm);

    // IPlayerProxy::Get()->Open("/sdcard/1080.mp4");
    // IPlayerProxy::Get()->Start();

    // IPlayerProxy::Get()->Open("/sdcard/v1080.mp4");
    // IPlayerProxy::Get()->Start();
    //de->Start();
    //vdecode->Start();
    //adecode->Start();
    ///XSleep(3000);
    //de->Stop();

    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT jstring

JNICALL
Java_xplay_xplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    // for test /////////////////
    

    ///////////////////////////////

    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_xplay_xplay_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    ANativeWindow* window = ANativeWindow_fromSurface( env, surface );
    IPlayerProxy::Get()->InitView(window);
}

extern "C"
JNIEXPORT void JNICALL
Java_xplay_xplay_OpenURL_Open(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    IPlayerProxy* player = IPlayerProxy::Get();
    player->Open( url );

    /* Start to play !*/
    player->Start();

    env->ReleaseStringUTFChars(url_, url);
}
extern "C"
JNIEXPORT void JNICALL
Java_xplay_xplay_MainActivity_jni_1seek(JNIEnv *env, jobject instance, jdouble progress) {

    IPlayerProxy* player = IPlayerProxy::Get();
    player->seek_update_progress( progress );
}extern "C"
JNIEXPORT jdouble JNICALL
Java_xplay_xplay_MainActivity_GetCurrProgress(JNIEnv *env, jobject instance) {

    IPlayerProxy* player = IPlayerProxy::Get();
    return player->curr_playing_position();
}