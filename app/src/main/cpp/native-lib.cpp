#include <jni.h>
#include <string>

#include "FFDemux.h" // for test
#include "XLog.h"

extern "C" JNIEXPORT jstring

JNICALL
Java_xplay_xplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    // for test //////////////////
    IDemux* de = new FFDemux;
    de->Open("/sdcard/1080.mp4");

    de->Start();
    ///////////////////////////////

    return env->NewStringUTF(hello.c_str());
}
