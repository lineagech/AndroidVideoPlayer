#include <jni.h>
#include <string>

#include "FFDemux.h"    // for test
#include "XLog.h"
#include "IDecode.h"    // for test
#include "FFDecode.h"   // for test

class TestObs: public IObserver{
public:
    void Update(XData d)
    {
        XLOGI("TestObs Update data is %d", d.size);
    }
};

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

    de->Start();
    vdecode->Start();
    adecode->Start();

    XSleep(3000);
    de->Stop();

    ///////////////////////////////

    return env->NewStringUTF(hello.c_str());
}
