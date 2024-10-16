#include <jni.h>
#include <string>
#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/input.h>
#define LOG_TAG "NativeInputSystem"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_test_1android_1kanji_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_test_1android_1kanji_MainActivity_handleKeyEventNative(JNIEnv *env, jobject obj,
                                                                        jint keycode,
                                                                        jint down) {
    if (down == AKEY_EVENT_ACTION_DOWN) {
        if (keycode >= AKEYCODE_0 && keycode <= AKEYCODE_9) {
            LOGI("Key pressed: %d", keycode - AKEYCODE_0);
            return JNI_TRUE;
        }
    }

    return JNI_FALSE;
}
