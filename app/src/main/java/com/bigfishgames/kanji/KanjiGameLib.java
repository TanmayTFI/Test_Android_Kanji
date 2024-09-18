package com.bigfishgames.kanji;
import android.content.res.AssetManager;

// Wrapper for native library

public class KanjiGameLib {

     static {
         System.loadLibrary("kanjigamelib");
     }

     public static native void setConfigVals(int jgameVersionCode, String jgameVersionName, int jgameOBBVersionCode,String jdeviceName,  boolean debugmode, int debugres, boolean splashskip, boolean surveybuild, int ln_id, int vid_res,String jfrf,String survey_url, String review_url, boolean cameraGrant, String pathExtrn);
     //public static native void setExtnName(String extMainPathName, String extPatchPathName);
     public static native void setFOrF(int unlockVal);
     public static native void getUserInfo(String dat, boolean isunlocked);
     public static native void setHints(String data);
     public static native int getNoOfHints();
     public static native void UnlockGame();
     public static native void ShowFullAssetDownloader();
     public static native void initWithAssetManager(AssetManager assetManager);
     public static native void surfaceCreated (KanjiGLSurfaceView.GLThread thd);
     public static native void shutdown (KanjiGLSurfaceView.GLThread thd);
     public static native void sizeChanged (KanjiGLSurfaceView.GLThread thd, int width, int height);
     public static native void handleTouchEvent (KanjiGLSurfaceView.GLThread thd, int nPointerId, int nPointerCount, float x, float y, float z, int nAction);
     public static native void handleKeyEvent (KanjiGLSurfaceView.GLThread thd, int keycode, int down, int unicode, int eventSource, int deviceId);
     public static native void handleAccelerometerEvent (KanjiGLSurfaceView.GLThread thd, float x, float y, float z);
     public static native void handleFocusEvent (KanjiGLSurfaceView.GLThread thd, int focusGained);
     public static native void handleUserEvent (KanjiGLSurfaceView.GLThread thd, int eventType);
     public static native void handleSoundEvent (KanjiGLSurfaceView.GLThread thd, int eventType, int eventData);
     public static native void handleJoystickEvent (KanjiGLSurfaceView.GLThread thd, float x, float y, float z, float rx, float ry, float rz,
                                                    float hatx, float haty, float hscroll, float vscroll,
                                                    float brake, float gas, float rudder, float throttle, float tilt, float wheel,
                                                    int unicode, int eventSource);
     public static native int getOpenGLESVersion (KanjiGLSurfaceView.GLThread thd);
     public static native void run(KanjiGLSurfaceView.GLThread thd);


}
