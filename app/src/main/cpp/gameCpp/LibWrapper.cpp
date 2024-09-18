#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>




extern void androidJniSetConfigVals(JNIEnv* env, jobject obj, int jgameVersionCode,jstring jgameVersionName, int jgameOBBVersionCode,jstring jdeviceName, jboolean debugmode, int debugres, jboolean splashskip, jboolean surveybuild, int ln_id, int vid_res, jstring jfrf, jstring survey_url, jstring review_url, jboolean cameraGrant,  jstring pathExtrn);
extern void androidJniSetFOrF(JNIEnv* env, jobject obj, int unlockVal);
extern void androidJniGetUserInfo(JNIEnv* env, jobject obj, jstring data, jboolean isunlocked);
extern void androidJniSetHints(JNIEnv* env, jobject obj, jstring data);
extern int androidJnigetNoOfHints(JNIEnv* env, jobject obj);
extern void androidJniUnlockGame(JNIEnv* env, jobject obj);
extern void androidJniShowFullAssetDownloader(JNIEnv* env, jobject obj);
extern void androidJniInitWithAssetManager(JNIEnv* env, jobject obj, jobject assetManager);
extern void androidJniSurfaceCreated (JNIEnv *env, jobject obj, jobject lpCaller, const char *lpszPackage);
extern void androidJniShutdown (JNIEnv *env, jobject obj, jobject lpCaller);
extern void androidJniSizeChanged (JNIEnv *env, jobject obj, jobject lpCaller, int width, int height);
extern void androidJniHandleTouchEvent (JNIEnv *env, jobject obj, jobject lpCaller, int nPointerId, int nPointerCount, float x, float y, float z, int nAction);
extern void androidJniHandleKeyEvent (JNIEnv *env, jobject obj, jobject lpCaller, int keycode, int down, int unicode, int eventSource, int deviceId);
extern void androidJniHandleAccelerometerEvent (JNIEnv *env, jobject obj, jobject lpCaller, float x, float y, float z);
extern void androidJniHandleFocusEvent (JNIEnv *env, jobject obj, jobject lpCaller, int nFocusGained);
extern void androidJniHandleUserEvent (JNIEnv *env, jobject obj, jobject lpCaller, int nEventType);
extern void androidJniHandleSoundEvent (JNIEnv *env, jobject obj, jobject lpCaller, int nEventType, int nEventData);
extern void androidJniHandleJoystickEvent (JNIEnv *env, jobject obj, jobject lpCaller, float x, float y, float z, float rx, float ry, float rz,
                                           float hatx, float haty, float hscroll, float vscroll,
                                           float brake, float gas, float rudder, float throttle, float tilt, float wheel,
                                           int eventSource, int deviceId);
extern int androidJniGetOpenGLESVersion (JNIEnv *env, jobject obj, jobject lpCaller);
extern void androidJniRun (JNIEnv *env, jobject obj, jobject lpCaller);

extern "C" {


   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_setConfigVals(JNIEnv* env, jobject obj, int jgameVersionCode,jstring jgameVersionName, int jgameOBBVersionCode,jstring jdeviceName, jboolean debugmode, int debugres, jboolean splashskip, jboolean surveybuild, int ln_id, int vid_res, jstring jfrf, jstring survey_url, jstring review_url,  jboolean cameraGrant, jstring pathExtrn );
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_setFOrF(JNIEnv* env, jobject obj, int unlockVal);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_getUserInfo(JNIEnv* env, jobject obj, jstring data, jboolean isunlocked);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_setHints(JNIEnv* env, jobject obj, jstring data);
   JNIEXPORT int JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_getNoOfHints(JNIEnv* env, jobject obj);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_UnlockGame(JNIEnv* env, jobject obj);

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_ShowFullAssetDownloader(JNIEnv* env, jobject obj);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_initWithAssetManager(JNIEnv* env, jobject obj, jobject assetManager);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_surfaceCreated(JNIEnv * env, jobject obj, jobject lpCaller);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_shutdown(JNIEnv * env, jobject obj, jobject lpCaller);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_sizeChanged(JNIEnv * env, jobject obj, jobject lpCaller, int width, int height);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleTouchEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nPointerId, int nPointerCount, float x, float y, float z, int nAction);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleKeyEvent(JNIEnv * env, jobject obj, jobject lpCaller, int keycode, int down, int unicode, int eventSource, int deviceId);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleAccelerometerEvent(JNIEnv * env, jobject obj, jobject lpCaller, float x, float y, float z);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleFocusEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nFocusGained);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleSoundEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nEventType, int nEventData);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleUserEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nEventType);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleJoystickEvent(JNIEnv * env, jobject obj, jobject lpCaller, float x, float y, float z, float rx, float ry, float rz,
                                                                                       float hatx, float haty, float hscroll, float vscroll,
                                                                                       float brake, float gas, float rudder, float throttle, float tilt, float wheel,
                                                                                       int eventSource, int deviceId);
   JNIEXPORT int JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_getOpenGLESVersion(JNIEnv * env, jobject obj, jobject lpCaller);
   JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_run(JNIEnv * env, jobject obj, jobject lpCaller);
};



JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_setConfigVals(JNIEnv* env, jobject obj,int jgameVersionCode,jstring jgameVersionName, int jgameOBBVersionCode, jstring jdeviceName, jboolean debugmode, int debugres, jboolean splashskip, jboolean surveybuild, int ln_id,int vid_res, jstring jfrf,jstring survey_url, jstring review_url, jboolean cameraGrant,  jstring pathExtrn) {
   androidJniSetConfigVals(env, obj, jgameVersionCode, jgameVersionName,jgameOBBVersionCode, jdeviceName, debugmode, debugres, splashskip, surveybuild, ln_id, vid_res, jfrf, survey_url, review_url, cameraGrant, pathExtrn);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_setFOrF(JNIEnv* env, jobject obj, int unlockVal) {
   androidJniSetFOrF(env, obj, unlockVal);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_getUserInfo(JNIEnv* env, jobject obj, jstring data, jboolean isunlocked) {
   androidJniGetUserInfo(env, obj, data, isunlocked);
}

JNIEXPORT int JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_getNoOfHints(JNIEnv* env, jobject obj) {
   return androidJnigetNoOfHints(env, obj);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_UnlockGame(JNIEnv* env, jobject obj) {
   androidJniUnlockGame(env, obj);
}


JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_ShowFullAssetDownloader(JNIEnv* env, jobject obj) {
   //androidJniShowFullAssetDownloader(env, obj);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_setHints(JNIEnv* env, jobject obj, jstring data) {
   androidJniSetHints(env, obj, data);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_initWithAssetManager(JNIEnv* env, jobject obj, jobject assetManager) {
   androidJniInitWithAssetManager (env, obj, assetManager);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_surfaceCreated(JNIEnv * env, jobject obj, jobject lpCaller) {
   androidJniSurfaceCreated (env, obj, lpCaller, "com/bigfishgames/kanji");
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_shutdown(JNIEnv * env, jobject obj, jobject lpCaller) {
   androidJniShutdown (env, obj, lpCaller);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_sizeChanged(JNIEnv * env, jobject obj, jobject lpCaller, int width, int height) {
   androidJniSizeChanged (env, obj, lpCaller, width, height);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleTouchEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nPointerId, int nPointerCount, float x, float y, float z, int nAction) {
   androidJniHandleTouchEvent (env, obj, lpCaller, nPointerId, nPointerCount, x, y, z, nAction);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleKeyEvent(JNIEnv * env, jobject obj, jobject lpCaller, int keycode, int down, int unicode, int eventSource, int deviceId) {
   androidJniHandleKeyEvent (env, obj, lpCaller, keycode, down, unicode, eventSource, deviceId);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleAccelerometerEvent(JNIEnv * env, jobject obj, jobject lpCaller, float x, float y, float z) {
   androidJniHandleAccelerometerEvent (env, obj, lpCaller, x, y, z);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleFocusEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nFocusGained) {
   androidJniHandleFocusEvent (env, obj, lpCaller, nFocusGained);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleSoundEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nEventType, int nEventData) {
   androidJniHandleSoundEvent (env, obj, lpCaller, nEventType, nEventData);   
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleUserEvent(JNIEnv * env, jobject obj, jobject lpCaller, int nEventType) {
   androidJniHandleUserEvent (env, obj, lpCaller, nEventType);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_handleJoystickEvent(JNIEnv * env, jobject obj, jobject lpCaller, float x, float y, float z, float rx, float ry, float rz,
                                                                                    float hatx, float haty, float hscroll, float vscroll,
                                                                                    float brake, float gas, float rudder, float throttle, float tilt, float wheel,
                                                                                    int eventSource, int deviceId) {
   androidJniHandleJoystickEvent (env, obj, lpCaller, x, y, z, rx, ry, rz,
                                  hatx, haty, hscroll, vscroll, brake, gas, rudder, throttle, tilt, wheel, eventSource, deviceId);
}

JNIEXPORT int JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_getOpenGLESVersion(JNIEnv * env, jobject obj, jobject lpCaller) {
   return androidJniGetOpenGLESVersion (env, obj, lpCaller);
}

JNIEXPORT void JNICALL Java_com_bigfishgames_kanji_KanjiGameLib_run(JNIEnv * env, jobject obj, jobject lpCaller) {
   androidJniRun (env, obj, lpCaller);

}
