/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This file was lifted from the APIDemos sample.  See: 
// http://developer.android.com/guide/samples/ApiDemos/src/com/example/android/apis/graphics/index.html
package com.bigfishgames.kanji;

import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.LinkedList;
import java.util.Arrays;
import java.util.Queue;
import java.util.Enumeration;
import java.util.UUID;
import java.util.Calendar;
import java.io.BufferedInputStream;
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileDescriptor;
import java.io.UnsupportedEncodingException;
import java.io.File;
import java.net.HttpURLConnection;
import java.net.NetworkInterface;
import java.net.InetAddress;
import java.net.Inet4Address;
import java.net.SocketException;
import java.net.URL;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.text.InputType;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.InputDevice;
import android.content.res.AssetManager;
import android.content.res.AssetFileDescriptor;
import android.content.SharedPreferences;
import android.content.DialogInterface;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.util.Log;
import android.net.Uri;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.app.ProgressDialog;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.os.Looper;
import android.os.Handler;
import android.os.Vibrator;
import android.os.VibrationEffect;
import android.os.Environment;
import android.os.StatFs;
import android.graphics.PixelFormat;
import android.view.Display;
import android.view.WindowManager;
import android.view.KeyEvent;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.ExtractedText;
import android.view.inputmethod.ExtractedTextRequest;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.Toast;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.WebSettings.ZoomDensity;
import android.provider.Settings.Secure;
import android.text.Html;

import androidx.core.content.ContextCompat;
import androidx.core.app.ActivityCompat;
import androidx.appcompat.app.AlertDialog;

import org.nativegame.NativeFrameClient;

/**
 * An implementation of SurfaceView that uses the dedicated surface for
 * displaying an OpenGL animation.  This allows the animation to run in a
 * separate thread, without requiring that it be driven by the update mechanism
 * of the view hierarchy.
 */
public class KanjiGLSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    public KanjiGLSurfaceView(Context context, AssetManager assetManager, Handler handler) {
        super(context);
        init(assetManager, handler);
    }

    public KanjiGLSurfaceView(Context context, AttributeSet attrs, AssetManager assetManager, Handler handler) {
        super(context, attrs);
        init(assetManager, handler);
    }

    private void init(AssetManager assetManager, Handler handler) {
        /* Check whether the game needs an alpha channel in the backbuffer or not */
        mBackbufferHasAlpha = false;
        try {
           ApplicationInfo appInfo = getContext().getPackageManager().getApplicationInfo (getContext().getPackageName(), PackageManager.GET_META_DATA);
           Object value = appInfo.metaData.get ("kanji-backbuffer-alpha");
           String strValue = value.toString ();
          
           if (strValue != null && strValue.equalsIgnoreCase ("yes")) {
              /* The game requested an alpha channel for the backbuffer */
              mBackbufferHasAlpha = true;
           }
        }
        catch (Exception e) {
        }
       
        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed
        mHolder = getHolder();
        mHolder.addCallback(this);
        if(android.os.Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN)
            mHolder.setType(SurfaceHolder.SURFACE_TYPE_GPU); // deprecated after API 16
        if (mBackbufferHasAlpha) mHolder.setFormat(PixelFormat.RGBA_8888);
        setFocusable (true);
        setFocusableInTouchMode (true);
        requestFocus ();
        mView = this;
        mIme = (InputMethodManager) getContext().getSystemService (Context.INPUT_METHOD_SERVICE);
        KanjiSound.setGameView (this);
        
        if (!mInitialized) {
            mGLThread = new GLThread(assetManager, handler);
            mGLThread.start();
           
            mInitialized = true;
        }
    }

    public SurfaceHolder getSurfaceHolder() {
        return mHolder;
    }

    public void setGLWrapper(GLWrapper glWrapper) {
        mGLWrapper = glWrapper;
    }

    public void surfaceCreated(SurfaceHolder holder) {
        mGLThread.surfaceCreated();
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        // Surface will be destroyed when we return
        mGLThread.surfaceDestroyed();
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        // Surface size or format has changed. This should not happen in this
        // example.
        mGLThread.onWindowResize(w, h);
    }
   
    public GLThread getGLThread () {
      return mGLThread;
    }

    public void onVirtualKeyboardShown() {
       Log.v ("Kanji", "virtual keyboard shown");
       mVirtualKeyboardDidShow = true;
    }
   
    public void onVirtualKeyboardHidden() {
       boolean kbdShowing = mVirtualKeyboardDidShow;
       
       mVirtualKeyboardDidShow = false;
       
       if (kbdShowing) {
          Log.v ("Kanji", "virtual keyboard hidden");
       }
       
       if (mVirtualKeyboardShown && kbdShowing) {
          Log.v ("Kanji", "send keyboard dismiss event");
          onUserEvent (113); /* K_EVENT_KEYBOARD_DISMISS */
       }
    }

    @Override
    protected void onMeasure (int widthMeasureSpec, int heightMeasureSpec) {
       super.onMeasure (widthMeasureSpec, heightMeasureSpec);
    }
  
    /* InputConnection for the soft keyboard */
    
    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
      outAttrs.initialCapsMode = 0;
      outAttrs.initialSelEnd = outAttrs.initialSelStart = -1;
      outAttrs.inputType = (InputType.TYPE_CLASS_TEXT|InputType.TYPE_TEXT_VARIATION_PASSWORD|InputType.TYPE_TEXT_FLAG_AUTO_COMPLETE|InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS);
      outAttrs.imeOptions = (EditorInfo.IME_ACTION_DONE |
                             EditorInfo.IME_FLAG_NO_EXTRACT_UI);
      
      return onLoadInputConnection();
    }
   
    protected InputConnection onLoadInputConnection() {
      return new TextInputConnection(this);
    }
   
    private class TextInputConnection extends BaseInputConnection {
      public TextInputConnection(KanjiGLSurfaceView view) {
         super(view, false);
      }
      
      @Override
      public ExtractedText getExtractedText (ExtractedTextRequest request, int flags) {
         ExtractedText et =  new ExtractedText ();
         
         et.text = "";
         return et;         
      }
       
      @Override
      public boolean performEditorAction(int actionCode) {
         return super.performEditorAction(actionCode);
      }
      
      
      @Override
      public boolean commitText(CharSequence text, int newCursorPosition) {
         int textLen = text.length (), i;
         
         for (i = 0; i < textLen; i++) {
            final int unicodeChar = text.charAt (i);
            
            /* Send keydown */
            setEvent(new Runnable() {
               public void run() {
                  KanjiGameLib.handleKeyEvent (mGLThread, 1234, 1, unicodeChar, 0, 0);
               }
            });
            
            /* Send keyup */
            setEvent(new Runnable() {
               public void run() {
                  KanjiGameLib.handleKeyEvent (mGLThread, 1234, 0, unicodeChar, 0, 0);
               }
            });
         }
         
         return false;
      }
       
      @Override
      public boolean setComposingText (CharSequence text, int newCursorPosition) {
         return false;
      }
       
      @Override
      public boolean deleteSurroundingText (int leftLength, int rightLength) {
         if (leftLength > 0) {
            /* Send keydown for backspace */
            setEvent(new Runnable() {
               public void run() {
                  KanjiGameLib.handleKeyEvent (mGLThread, 67 /* Android backspace keycode */, 1, 8 /* Unicode */, 0, 0);
               }
            });
            
            /* Send keyup for backspace */
            setEvent(new Runnable() {
               public void run() {
                  KanjiGameLib.handleKeyEvent (mGLThread, 67, 0, 8, 0, 0);
               }
            });            
         }
         return super.deleteSurroundingText (leftLength, rightLength);
      }
       
      @Override
      public boolean sendKeyEvent (KeyEvent keyEvent) {
         return super.sendKeyEvent (keyEvent);
      }
    }
   
    /**
     * Inform the view that the activity is paused.
     */
    public void onPause() {
        mGLThread.onPause();
    }

    /**
     * Inform the view that the activity is resumed.
     */
    public void onResume() {
        mGLThread.onResume();
    }
    
    /**
     * Send user event to game
     *
     * \param eventType type of event to send
     */
    public void onUserEvent (int eventType) {
       final int finalEventType = eventType;
       
       setEvent(new Runnable() {
           public void run() {
               if (finalEventType == 113) {
                  /* K_EVENT_KEYBOARD_DISMISS -- make sure the virtual keyboard is hidden */
                  mGLThread.androidSetVirtualKeyboardState (0);
               }
               KanjiGameLib.handleUserEvent (mGLThread, finalEventType);
           }
       });    
    }
   
    /**
     * Send periodic audio notification to game
     *
     * \param nNotificationUserData opaque data to pass back to the game
     */
    public void onPeriodicAudioNotification (final int nNotificationUserData) {
       /* Call in the context of the periodic notification thread, not the GLThread, so
        * that audio buffers are refilled even if the game thread is busy */
       KanjiGameLib.handleSoundEvent (mGLThread, 1, nNotificationUserData);
    }
   
    /**
     * Send local notification event to game
     *
     * \param nUserData user data for notification, passed back to the game
     */
    public void onLocalNotificationEvent (int nUserData) {
       final int finalUserData = nUserData;
       
       setEvent(new Runnable() {
          public void run() {
             mLocalNotificationUserData = finalUserData;
             KanjiGameLib.handleUserEvent (mGLThread, 123 /* K_EVENT_LOCAL_NOTIFICATION */);
             mLocalNotificationUserData = 0;
          }
       });
    }
   
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
       Log.d("Kanji", "onRequestPermissionsResult: requestCode="+requestCode);
       
       if (requestCode == 100 && mSemBlockPermissions != null) {
          boolean bGranted = false;
          
          if (grantResults.length > 0) {
             int i;
             
             bGranted = true;
             for (i = 0; i < grantResults.length; i++) {
                if (grantResults[i] != PackageManager.PERMISSION_GRANTED) {
                   Log.d("Kanji", "permission not granted: " + permissions[i]);
                   bGranted = false;
                }
             }
          }
          
          Log.d("Kanji", "permission grant result: " + bGranted);
          mRequestPermissionsResult = bGranted;
          mSemBlockPermissions.release();
       }
    }
   
    /**
     * Check if this view needs an orientation fix now (work around a bug coming out of device sleep)
     *
     * \return true if needed now, false if not
     */
    public boolean needOrientationFixNow () {
       return mNeedOrientationFix && !mPaused && mHasFocus && mHasSurface && !mContextLost && !mDone;
    }
   
    /**
     * Mark that the view orientation has been fixed
     */
    public void orientationFixed () {
      mNeedOrientationFix = false;
    }
   
    private boolean mTouchPressureSupported = false;
   
    public boolean onTouchEvent(MotionEvent touchEvent) {
       final MotionEvent event = MotionEvent.obtain (touchEvent);
      
       setEvent(new Runnable() {
          public void run() {
             int nAction = 0, i, nPointerCount;
       
             switch (event.getAction () & MotionEvent.ACTION_MASK) {
             case MotionEvent.ACTION_DOWN:
                nAction = 1;
                break;
             case MotionEvent.ACTION_MOVE:
                nAction = 2;
                break;
             case MotionEvent.ACTION_UP:
                nAction = 3;
                break;
             case MotionEvent.ACTION_CANCEL:
                nAction = 4;
                break;
             default:
                break;
             }
             
             nPointerCount = event.getPointerCount();
             for (i = 0; i < nPointerCount; i++) {
                float fPressure = event.getPressure(i);
                if (fPressure < 1) mTouchPressureSupported = true;
                
                if (!mTouchPressureSupported) fPressure = 0;
                fPressure = fPressure / 2;
                if (fPressure < 0) fPressure = 0;
                if (fPressure > 1) fPressure = 1;
                KanjiGameLib.handleTouchEvent (mGLThread, event.getPointerId(i), nPointerCount, event.getX(i), event.getY(i), fPressure, nAction);
             }
          }
       });
       return true;
    }

    public boolean onKeyDown(int keyCode, final KeyEvent event) {
       final int finalKeyCode = keyCode;
       
       if (android.os.Build.VERSION.SDK_INT >= 9) {
          /* getSource() is only supported in API level 9 and above */
          setEvent(new Runnable() {
             public void run() {
                KanjiGameLib.handleKeyEvent (mGLThread, finalKeyCode, 1, event.getUnicodeChar(), event.getSource(), event.getDeviceId());
             }
          });
       }
       else {
          setEvent(new Runnable() {
             public void run() {
                KanjiGameLib.handleKeyEvent (mGLThread, finalKeyCode, 1, event.getUnicodeChar(), 0, 0);
             }
          });
       }
       
       if (mCatchBackKey && (keyCode == KeyEvent.KEYCODE_BACK || (keyCode >= 96 && keyCode <= 110)))
          return true;
       
       return false;
    }
    
    public boolean onKeyUp(int keyCode, final KeyEvent event) {
       final int finalKeyCode = keyCode;
       
       if (android.os.Build.VERSION.SDK_INT >= 9) {
          /* getSource() is only supported in API level 9 and above */
          setEvent(new Runnable() {
             public void run() {
                KanjiGameLib.handleKeyEvent (mGLThread, finalKeyCode, 0, event.getUnicodeChar(), event.getSource(), event.getDeviceId());
             }
          });
       }
       else {
          setEvent(new Runnable() {
             public void run() {
                KanjiGameLib.handleKeyEvent (mGLThread, finalKeyCode, 0, event.getUnicodeChar(), 0, 0);
             }
          });
       }
       
       if (mCatchBackKey && (keyCode == KeyEvent.KEYCODE_BACK || (keyCode >= 96 && keyCode <= 110)))
          return true;
       
       return false;
    }
   
    public boolean onGenericMotionEvent(final MotionEvent event) {
       if (android.os.Build.VERSION.SDK_INT >= 12) {
          /* getSource() is only supported in API level 9 and above. The joystick HID support and getAxisValue() are only supported in API level 12 and above */
          if ((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0) {
             setEvent(new Runnable() {
                public void run() {
                   KanjiGameLib.handleJoystickEvent (mGLThread, event.getAxisValue (MotionEvent.AXIS_X), event.getAxisValue (MotionEvent.AXIS_Y), event.getAxisValue (MotionEvent.AXIS_Z),
                                                     event.getAxisValue (MotionEvent.AXIS_RX), event.getAxisValue (MotionEvent.AXIS_RY), event.getAxisValue (MotionEvent.AXIS_RZ),
                                                     event.getAxisValue (MotionEvent.AXIS_HAT_X), event.getAxisValue (MotionEvent.AXIS_HAT_Y),
                                                     event.getAxisValue (MotionEvent.AXIS_HSCROLL), event.getAxisValue (MotionEvent.AXIS_VSCROLL),
                                                     event.getAxisValue (MotionEvent.AXIS_BRAKE), event.getAxisValue (MotionEvent.AXIS_GAS),
                                                     event.getAxisValue (MotionEvent.AXIS_RUDDER), event.getAxisValue (MotionEvent.AXIS_THROTTLE),
                                                     event.getAxisValue (MotionEvent.AXIS_TILT), event.getAxisValue (MotionEvent.AXIS_WHEEL),
                                                     event.getSource(), event.getDeviceId());
                }
             });
          }
       }
       
       return false;    /* So as to get d-pad key events from the joysticks as well */
    }
   
    public boolean onAccelerometer(float x, float y, float z) {
        final float finalX = x;
        final float finalY = y;
        final float finalZ = z;

        setEvent(new Runnable() {
          public void run() {
            KanjiGameLib.handleAccelerometerEvent (mGLThread, finalX, finalY, finalZ);
          }
        });

        return false;
    }

    public void setKanjiInFront (boolean bInFront) {
       setZOrderOnTop (bInFront);
    }
   
    /**
     * Inform the view that the window focus has changed.
     */
    @Override public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        mGLThread.onWindowFocusChanged(hasFocus);
    }

    /**
     * Set an "event" to be run on the GL rendering thread.
     * @param r the runnable to be run on the GL rendering thread.
     */
    public void setEvent(Runnable r) {
        mGLThread.setEvent(r);
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
    }

    // ----------------------------------------------------------------------

	 private boolean mReportNextFrameStart = true;
	 private boolean mReportNextFrameEnd = false;
	
	 public void frameStart() {
		if (mReportNextFrameStart) {			
			mReportNextFrameStart = false;
			mReportNextFrameEnd = true;
			
         NativeFrameClient.OnFrameStartCallback onFrameStartCallback = NativeFrameClient.getOnFrameStartCallback ();
			if(onFrameStartCallback != null) {
				onFrameStartCallback.onFrameStart(mWidth, mHeight);
			}
		}
	 }

	 public void frameEnd() {
		if (mReportNextFrameEnd) {
			mReportNextFrameEnd = false;
         mReportNextFrameStart = true;
			
         NativeFrameClient.OnFrameEndCallback onFrameEndCallback = NativeFrameClient.getOnFrameEndCallback ();
			if(onFrameEndCallback != null){
				onFrameEndCallback.onFrameEnd();
			}
		}
	 }

    // ----------------------------------------------------------------------

    public interface GLWrapper {
      GL wrap(GL gl);
    }

    // ----------------------------------------------------------------------

     /**
     * An EGL helper class.
     */

    private class EglHelper {
        public EglHelper() {

        }

        /**
         * Initialize EGL for a given configuration spec.
         * @param configSpec
         */
        public void start(int[] configSpec, int nOpenGLESVersion){
            /*
             * Get an EGL instance
             */
            mEgl = (EGL10) EGLContext.getEGL();

            /*
             * Get to the default display.
             */
            mEglDisplay = mEgl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);

            /*
             * We can now initialize EGL for that display
             */
            int[] version = new int[2];
            mEgl.eglInitialize(mEglDisplay, version);

            EGLConfig[] configs = new EGLConfig[1];
            int[] num_config = new int[1];
            mEgl.eglChooseConfig(mEglDisplay, configSpec, configs, 1,
                    num_config);
            mEglConfig = configs[0];

            /*
            * Create an OpenGL ES context. This must be done only once, an
            * OpenGL context is a somewhat heavy object.
            */
            if (nOpenGLESVersion == 2) {
               final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
               int[] nOpenGLES2AttribList = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
               mEglContext = mEgl.eglCreateContext(mEglDisplay, mEglConfig,
                                                   EGL10.EGL_NO_CONTEXT, nOpenGLES2AttribList);
            }
            else {
               mEglContext = mEgl.eglCreateContext(mEglDisplay, mEglConfig,
                       EGL10.EGL_NO_CONTEXT, null);
            }

            mEglSurface = null;
            checkMultipleEGLContextsSupport ();
        }

        /*
         * Create and return an OpenGL surface
         */
        public GL createSurface(SurfaceHolder holder) {
            /*
             *  The window size has changed, so we need to create a new
             *  surface.
             */
            if (mEglSurface != null) {

                /*
                 * Unbind and destroy the old EGL surface, if
                 * there is one.
                 */
                mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
                        EGL10.EGL_NO_SURFACE, EGL10.EGL_NO_CONTEXT);
                mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
            }

            /*
             * Create an EGL surface we can render into.
             */
            mEglSurface = mEgl.eglCreateWindowSurface(mEglDisplay,
                    mEglConfig, holder, null);

            /*
             * Before we can issue GL commands, we need to make sure
             * the context is current and bound to a surface.
             */
            mEgl.eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
                    mEglContext);


            GL gl = mEglContext.getGL();
            if (mGLWrapper != null) {
                gl = mGLWrapper.wrap(gl);
            }
            return gl;
        }

        /**
         * Display the current render surface.
         * @return false if the context has been lost.
         */
        public boolean swap() {
            if (mEgl != null && mEglSurface != null && mEglContext != null && mEglDisplay != null) {
               mEgl.eglSwapBuffers(mEglDisplay, mEglSurface);

               /*
                * Always check for EGL_CONTEXT_LOST, which means the context
                * and all associated data were lost (For instance because
                * the device went to sleep). We need to sleep until we
                * get a new surface.
                */
               return mEgl.eglGetError() != EGL11.EGL_CONTEXT_LOST;
            }
            else {
               /* No active context */
               return false;
            }
        }
       
        public boolean hasContext() {
           return mEgl != null && mEglSurface != null && mEglContext != null && mEglDisplay != null;
        }

        public void finish() {
            if (mEglSurface != null) {
                mEgl.eglMakeCurrent(mEglDisplay, EGL10.EGL_NO_SURFACE,
                        EGL10.EGL_NO_SURFACE,
                        EGL10.EGL_NO_CONTEXT);
                mEgl.eglDestroySurface(mEglDisplay, mEglSurface);
                mEglSurface = null;
            }
            if (mEglContext != null) {
                mEgl.eglDestroyContext(mEglDisplay, mEglContext);
                mEglContext = null;
            }
            if (mEglDisplay != null) {
                mEgl.eglTerminate(mEglDisplay);
                mEglDisplay = null;
            }
        }

        public boolean areMultipleEGLContextsSupported() {
           return mMultipleEGLContextsSupported;
        }
       
        private void checkMultipleEGLContextsSupport() {
           mMultipleEGLContextsSupported = false;
          
           try {
              /* Only trust multiple contexts support on Honeycomb and above */
              if (android.os.Build.VERSION.SDK_INT >= 11) {
                 ActivityManager activityManager = (ActivityManager) getContext().getSystemService(Context.ACTIVITY_SERVICE);
                 ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo ();
                 int nGLESVersion = configurationInfo.reqGlEsVersion;
                
                 Log.v ("Kanji", "device OpenGL ES version: 0x" + Integer.toHexString(nGLESVersion));
                 if (nGLESVersion >= 0x20000) {
                    mMultipleEGLContextsSupported = true;
                 }
              }
           } catch (Exception e) {
           }
           
           if (mMultipleEGLContextsSupported)
              Log.v ("Kanji", "multiple EGL contexts supported");
           else
              Log.v ("Kanji", "multiple EGL contexts not supported");
        }
       
        EGL10 mEgl;
        EGLDisplay mEglDisplay;
        EGLSurface mEglSurface;
        EGLConfig mEglConfig;
        EGLContext mEglContext;
        boolean mMultipleEGLContextsSupported = false;
    }

    /**
     * A generic GL Thread. Takes care of initializing EGL and GL
     */

    public class GLThread extends Thread {
        final int EGL_OPENGL_ES2_BIT = 4;
       
        GLThread(AssetManager assetManager, Handler handler) {
            super();
            mAssetManager = assetManager;
            mHandler = handler;
            mDone = false;
            mWidth = 0;
            mHeight = 0;
            setName("GLThread");
        }

        @Override
        public void run() {
            /*
             * When the android framework launches a second instance of
             * an activity, the new instance's onCreate() method may be
             * called before the first instance returns from onDestroy().
             *
             * This semaphore ensures that only one instance at a time
             * accesses EGL.
             */
            try {
                try {
                sEglSemaphore.acquire();
                } catch (InterruptedException e) {
                    return;
                }
                guardedRun();
            } catch (InterruptedException e) {
                // fall thru and exit normally
            } finally {
                sEglSemaphore.release();
            }
           
            try {
               ((Activity) getContext()).finish();
               Thread.sleep (100);
               android.os.Process.killProcess (android.os.Process.myPid ());
            } catch (Exception e) {
               android.os.Process.killProcess (android.os.Process.myPid ());
            }
        }

        private String readTextFile(InputStream inputStream) {  
            ByteArrayOutputStream outputStream = new ByteArrayOutputStream();  
            byte buf[] = new byte[1024];  
            int len;  
            try {  
                while ((len = inputStream.read(buf)) != -1) {  
                    outputStream.write(buf, 0, len);  
                }  
                outputStream.close();  
                inputStream.close();  
            } catch (IOException e) {  
            }  
            return outputStream.toString();  
        }  

        private void guardedRun() throws InterruptedException {
            int nOpenGLESVersion = KanjiGameLib.getOpenGLESVersion(this);
         
            mEglHelper = new EglHelper();
           
            /*
             * Specify a configuration for our opengl session
             * and grab the first configuration that matches it
             */
            int[] configSpecGLES2 = { EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };
            int[] configSpecGLES2WithAlpha = { EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL10.EGL_ALPHA_SIZE, 8, EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };
            int[] configSpecGLES1 = { EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };
            int[] configSpecGLES1WithAlpha = { EGL10.EGL_ALPHA_SIZE, 8, EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };
            if (nOpenGLESVersion == 2) {
               Log.v ("Kanji", "Requesting OpenGL ES 2.0");
               if (mBackbufferHasAlpha)
                  mEglHelper.start(configSpecGLES2WithAlpha, nOpenGLESVersion);
               else
                  mEglHelper.start(configSpecGLES2, nOpenGLESVersion);
            }
            else {
               Log.v ("Kanji", "Requesting OpenGL ES 1.x");
               if (mBackbufferHasAlpha)
                  mEglHelper.start(configSpecGLES1WithAlpha, nOpenGLESVersion);
               else
                  mEglHelper.start(configSpecGLES1, nOpenGLESVersion);
            }

            mGl = null;
            mTellRendererSurfaceCreated = true;
            mTellRendererSurfaceChanged = true;
           
            do {
                processEvents ();
            } while (mCurWidth <= 0 || mCurHeight <= 0);
           
            mPreserveEGLContextOnPause = false;
            try {
               ApplicationInfo appInfo = getContext().getPackageManager().getApplicationInfo (getContext().getPackageName(), PackageManager.GET_META_DATA);
               Object value = appInfo.metaData.get ("kanji-preserve-glcontext");
               String strValue = value.toString ();
               
               if (strValue != null && strValue.equalsIgnoreCase ("yes")) {
                  /* The game requested that we attempt to preserve the GL context when the app is paused */
                  mPreserveEGLContextOnPause = true;
               }
            }
            catch (Exception e) {
            }
           
            Looper.prepare ();

            // Enable HTTP response cache if possible
            try {
               long httpCacheSize = 10 * 1024 * 1024; // 10 MiB
               File httpCacheDir = new File(getContext().getCacheDir(), "http");
               Class.forName("android.net.http.HttpResponseCache")
                        .getMethod("install", File.class, long.class)
                        .invoke(null, httpCacheDir, httpCacheSize);
               Log.v("Kanji", "HTTP response cache enabled");
            }
            catch (Throwable e) {
               Log.v("Kanji", "HTTP response cache is unavailable.");
            }

            KanjiGameLib.run(this);
            mEglHelper.finish();
            Runtime.getRuntime().gc();
        }
       
        public Integer androidGetScreenRotation () {
            if (mEglHelper != null && mEglHelper.hasContext () && !mPaused) {
               WindowManager wm = (WindowManager) getContext().getSystemService(Context.WINDOW_SERVICE);
               if (wm != null) {
                  Display display = wm.getDefaultDisplay();

                  if (display != null)
                   //  mRotation = display.getOrientation (); // deprecated from API 8,
                      mRotation = display.getRotation ();
               }
            }
            return mRotation;
        }
       
        public Integer androidGetScreenInches () {
           Integer nInches = -1;
           
           if (mEglHelper != null && mEglHelper.hasContext () && !mPaused) {
              WindowManager wm = (WindowManager) getContext().getSystemService(Context.WINDOW_SERVICE);
              if (wm != null) {
                 Display display = wm.getDefaultDisplay();
                 
                 if (display != null) {
                    DisplayMetrics dm = new DisplayMetrics ();
                    double x, y;
                    Double fInches;
                    
                    display.getMetrics (dm);
                    x = Math.pow (dm.widthPixels / dm.xdpi, 2);
                    y = Math.pow (dm.heightPixels / dm.ydpi, 2);
                    fInches = Math.sqrt (x + y);
                    nInches = (Integer) fInches.intValue();
                 }
              }
           }
           
           return nInches;
        }
       
        public Integer androidGetDeviceEventSources (int nDeviceId) {
           int nSources =  0;
          
           if (android.os.Build.VERSION.SDK_INT >= 9) {
              /* InputDevice is only supported in API level 9 or above */
              try {
                 InputDevice inputDevice = InputDevice.getDevice (nDeviceId);
                 nSources = inputDevice.getSources ();
              } catch (Exception e) {
              }
           }
          
           return (Integer) nSources;
        }
       
        public String androidGetPackageMetadata (String strPropertyName) {
           String strValue;
           
           try {
              ApplicationInfo appInfo = getContext().getPackageManager().getApplicationInfo (getContext().getPackageName(), PackageManager.GET_META_DATA);
              Object value = appInfo.metaData.get (strPropertyName);
              
              strValue = value.toString ();
           }
           catch (Exception e) {
              strValue = "";
           }
           
           return strValue;
        }

        public String flipGlBuffers () {
           if (!mDone && mHasSurface && !mPaused) {
              frameEnd ();
              mEglHelper.swap();
              frameStart ();
           }
           return null;
        }
       
        public String collectGarbage () {
           Runtime.getRuntime().gc();
           return null;
        }
        
        public void enableNativeEventsDelivery () {
           mDeliverNativeEvents = true;
        }
       
        public String processEvents () {
           try {
                /*
                 *  Update the asynchronous state (window size)
                 */
                boolean changed;
                boolean needStart = false;
                Runnable r;
              
                if (mDeliverNativeEvents) {
                   do {
                       synchronized (this) {
                          try {
                             r = (Runnable) (mEventQueue.remove ());
                          } catch (java.util.NoSuchElementException e) {
                             r = null;
                          }
                       }
                      
                       if (r != null) {
                          r.run();
                       }
                   } while (r != null);
                }
              
                synchronized (this) {
                    if (mNotifyPause) {                       
                        // Dispatch deactivation event to game
                        KanjiGameLib.handleFocusEvent (mGLThread, 0);
                       
                        mHandler.post (new Runnable() {
                          public void run() {
                              if (mProgressDialog != null) {
                                 // Remove progress dialog and flag to re-create it after reactivating the game
                                 mProgressDialog.dismiss ();
                                 mProgressDialog = null;
                                 mRestoreProgressDialog = true;
                              }
                          }
                        });
                       
                        if (mVirtualKeyboardShown) {
                           // Hide keyboard, flag to show it again when reactivating the game
                           androidSetVirtualKeyboardState (0);
                           mVirtualKeyboardDidShow = false;
                           mRestoreKeyboard = true;
                        }
                        
                        if (!mPreserveEGLContextOnPause || !mEglHelper.areMultipleEGLContextsSupported()) {
                           // Destroy GL context
                           mEglHelper.finish();
                           needStart = true;
                        }
                        else {
                           mTellRendererSurfaceChanged = true;
                        }
                        mNeedOrientationFix = true;
                        mNotifyPause = false;
                    }
                   
                    if(needToWait()) {
                        while (needToWait()) {
                            if (mNotifyPause) {                              
                               // Dispatch deactivation event to game
                               KanjiGameLib.handleFocusEvent (mGLThread, 0);
                              
                               if (!mPreserveEGLContextOnPause || !mEglHelper.areMultipleEGLContextsSupported()) {
                                  // Destroy GL context
                                  mEglHelper.finish();
                                  needStart = true;
                               }
                               else {
                                  mTellRendererSurfaceChanged = true;
                               }
                               mNeedOrientationFix = true;
                               mNotifyPause = false;
                            }
                           
                            wait();
                        }
                    }
                    if (mDone) {
                        KanjiGameLib.shutdown (this);
                        return null;
                    }
                    changed = mSizeChanged;
                    mCurWidth = mWidth;
                    mCurHeight = mHeight;
                    mSizeChanged = false;
                }
                if (needStart) {
                    int nOpenGLESVersion = KanjiGameLib.getOpenGLESVersion(this);
                    int[] configSpecGLES2 = { EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };
                    int[] configSpecGLES2WithAlpha = { EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL10.EGL_ALPHA_SIZE, 8, EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };
                    int[] configSpecGLES1 = { EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };
                    int[] configSpecGLES1WithAlpha = { EGL10.EGL_ALPHA_SIZE, 8, EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_STENCIL_SIZE, 8, EGL10.EGL_NONE };
                    if (nOpenGLESVersion == 2) {
                       Log.v ("Kanji", "requesting OpenGL ES 2.0");
                       if (mBackbufferHasAlpha)
                          mEglHelper.start(configSpecGLES2WithAlpha, nOpenGLESVersion);
                       else
                          mEglHelper.start(configSpecGLES2, nOpenGLESVersion);
                    }
                    else {
                       Log.v ("Kanji", "requesting OpenGL ES 1.x");
                       if (mBackbufferHasAlpha)
                          mEglHelper.start(configSpecGLES1WithAlpha, nOpenGLESVersion);
                       else
                          mEglHelper.start(configSpecGLES1, nOpenGLESVersion);
                    }
                   
                    mTellRendererSurfaceCreated = true;
                    mReloadNeeded = true;
                    changed = true;
                }
                if (changed) {
                    mGl = (GL10) mEglHelper.createSurface(mHolder);
                    mTellRendererSurfaceChanged = true;
                }
                if (mTellRendererSurfaceCreated) {
                    Log.v ("Kanji", "surface created");
                    KanjiGameLib.surfaceCreated(this);
                    mTellRendererSurfaceCreated = false;
                }
                if (mTellRendererSurfaceChanged) {
                    Log.v ("Kanji", "surface changed");
                    KanjiGameLib.sizeChanged(this, mCurWidth, mCurHeight);
                    mTellRendererSurfaceChanged = false;

                    NativeFrameClient.OnSurfaceChangedCallback onSurfaceChangedCallback = NativeFrameClient.getOnSurfaceChangedCallback ();
			           if (onSurfaceChangedCallback != null) {
				           onSurfaceChangedCallback.onSurfaceChanged(mView, mCurWidth, mCurHeight);
			           }
                   
                    if (mNotifyResume) {
                      mNotifyResume = false;
                                            
                      // Dispatch activation event to game
                      KanjiGameLib.handleFocusEvent (mGLThread, mReloadNeeded ? 1 : 2);
                      mReloadNeeded = false;
                      mFocusLost = false;
                      
                      mHandler.post (new Runnable() {
                         public void run() {
                            SurfaceView tempView;
                            
                            tempView = new SurfaceView ((Activity) getContext());
                            ((Activity) getContext()).addContentView (tempView, new LayoutParams (LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT)); // FILL_PARENT is deprecated, use MATCH_PARENT
                            tempView.setZOrderMediaOverlay (true);
                            setZOrderOnTop (false);
                            ((ViewGroup) tempView.getParent()).removeView (tempView);
                            tempView = null;
                            
                            if (mRestoreProgressDialog) {
                               // Restore busy wheel
                               
                               if (mProgressDialog != null) {
                                  mProgressDialog.dismiss ();
                                  mProgressDialog = null;
                               }
                               mProgressDialog = new ProgressDialog (getContext());
                               mProgressDialog.setMessage (mProgressDialogMessage);
                               mProgressDialog.setIndeterminate (true);
                               mProgressDialog.setCancelable (false);
                               mProgressDialog.show ();
                               mRestoreProgressDialog = false;
                            }
                         }
                      });
                   }
                }
              
               if (mFocusLost) {
                  mFocusLost = false;
                  
                  Log.v ("Kanji", "restore Z-order");
                  KanjiGameLib.handleFocusEvent (mGLThread, 0);
                  KanjiGameLib.handleFocusEvent (mGLThread, 2);
                  mHandler.post (new Runnable() {
                     public void run() {
                        SurfaceView tempView;
                        
                        tempView = new SurfaceView ((Activity) getContext());
                        ((Activity) getContext()).addContentView (tempView, new LayoutParams (LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));// FILL_PARENT is deprecated, use MATCH_PARENT
                        tempView.setZOrderMediaOverlay (true);
                        setZOrderOnTop (false);
                        ((ViewGroup) tempView.getParent()).removeView (tempView);
                        tempView = null;
                     }
                  });
               }
            } catch (InterruptedException e) {
            }
            
            return null;
        }
       
        public void onFocusLost () {
          mFocusLost = true;
        }
       
        public String androidSetVirtualKeyboardState (int nShow) {
            InputMethodManager mgr = (InputMethodManager) getContext().getSystemService (Context.INPUT_METHOD_SERVICE);
          
            if (mgr != null) {
                if (nShow == 2) {
                   if (mRestoreKeyboard) {
                      // Restore keyboard
                      Log.v ("Kanji", "restore keyboard upon resume");
                      mRestoreKeyboard = false;
                      androidSetVirtualKeyboardState (1);
                   }
                }
                else if (nShow == 1) {
                    if (!mVirtualKeyboardShown) {
                        Log.v ("Kanji", "request to show keyboard");
                        mVirtualKeyboardShown = true;
                        mgr.showSoftInput (mView, 0);
                        mView.requestFocus ();
                    }
                }
                else {
                    if (mVirtualKeyboardShown) {
                        Log.v ("Kanji", "request to hide keyboard");
                        mVirtualKeyboardShown = false;
                        mgr.hideSoftInputFromWindow (mView.getWindowToken(), 0);
                    }
                }
            }
          
            return null;
        }
       
        public String androidCatchBackKey (int nCatch) {
           mCatchBackKey = (nCatch != 0) ? true : false;
           return null;
        }
       
        public String androidSendToBack () {
           ((Activity) getContext()).moveTaskToBack (true);  
           return null;
        }
       
        public String androidDisableIdleTimer (int nDisable) {
           if (nDisable != 0) {
              Runnable r = new Runnable () {
                 public void run() {
                    ((Activity) getContext()).getWindow().setFlags (WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                                                                    WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);      
                 }
              };
              
              mHandler.post (r);         
           }
           else {
              Runnable r = new Runnable () {
                 public void run() {
                    ((Activity) getContext()).getWindow().clearFlags (WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);      
                 }
              };
              
              mHandler.post (r);         
           }
           
           return null;
        }  
       
        public class KanjiAndroidFile {
            public FileDescriptor fd;
            public long nStartOffset;
            public long nLength;
        }

        public String logAndroidMessage (String msg) {
            Log.v ("Kanji", msg);
            return null;
        }

        public KanjiAndroidFile openAndroidFile (String filename) {
            try {
                AssetFileDescriptor afd;
                KanjiAndroidFile f;

                afd = mAssetManager.openFd (filename);
                f = new KanjiAndroidFile ();
                f.fd = afd.getFileDescriptor ();
                f.nStartOffset = afd.getStartOffset ();
                f.nLength = afd.getLength ();
                return f;
            } catch (IOException e) {
            }

            return null;
        }

        public String androidGetFilesDir () {
            return getContext().getFilesDir().getAbsolutePath ();
        }

        public String androidGetOSString (int nStringIdx) {
            UUID uuid;
            String storageState, storagePath, versionStr, obbPath, freeSpace, memSize;
           
            switch (nStringIdx) {
            case 0:
                return android.os.Build.VERSION.RELEASE;

            case 1:
                return android.os.Build.MANUFACTURER;
         
            case 2:
                return android.os.Build.DEVICE;
         
            case 3:
                return android.os.Build.MODEL;
         
            case 4:    
                return android.os.Build.PRODUCT;
         
            case 5:               
                final String PREFS_FILE = "kanji_device_id";
                final String PREFS_DEVICE_ID = "device_id_str";
                Context context = getContext ();
                final SharedPreferences prefs = context.getSharedPreferences (PREFS_FILE, 0);
                final String id = prefs.getString(PREFS_DEVICE_ID, null );
                  
                uuid = UUID.randomUUID();
                  
                if (id != null) {
                    /* Read previously stored ID */
                    Log.v ("Kanji", "Device ID: read from preferences");
                    uuid = UUID.fromString(id);
                }
                else {
                    final String androidId = Secure.getString(context.getContentResolver(), Secure.ANDROID_ID);
                     
                    Log.v ("Kanji", "Device ID: ANDROID_ID=" + androidId);                     
                    try {
                        if (androidId != null && !"9774d56d682e549c".equals(androidId)) {
                            /* Valid ID */
                            Log.v ("Kanji", "Device ID: use ANDROID_ID");
                            uuid = UUID.nameUUIDFromBytes(androidId.getBytes("utf8"));
                        }
                        else {
                            /* Null ID (market app has never been ran), emulator, or broken device ID; use a random number and store it */
                            Log.v ("Kanji", "Device ID: use random number");
                            uuid = UUID.randomUUID();
                        }
                    } catch (UnsupportedEncodingException e) {
                    }
                    
                    prefs.edit().putString (PREFS_DEVICE_ID, uuid.toString()).commit();                     
                }
                  
                return uuid.toString();
            
            case 6:
                try {
                    for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
                        NetworkInterface intf = en.nextElement();
                        for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                            InetAddress inetAddress = enumIpAddr.nextElement();
                           
                           if (inetAddress instanceof Inet4Address) {
                               if (!inetAddress.isLoopbackAddress()) {
                                   return inetAddress.getHostAddress().toString();
                               }
                           }
                        }
                    }
                } catch (SocketException ex) {
                   Log.v ("Kanji", "No permission to access the Internet, missing android.permission.INTERNET");
                }
                return "";
                  
            case 7:
                // locale is deprecated in configuration after API 24
                //return getContext().getResources().getConfiguration().locale.getISO3Language();
                return  android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.N ?getContext().getResources().getConfiguration().getLocales().get(0).getISO3Language():getContext().getResources().getConfiguration().locale.getISO3Language();
            case 8:
               storagePath = getContext().getFilesDir().getAbsolutePath ();
               if (android.os.Build.VERSION.SDK_INT >= 8) {
                  try {
                     storageState = Environment.getExternalStorageState ();
                     if (Environment.MEDIA_MOUNTED.equals (storageState))
                        storagePath = getContext().getExternalFilesDir (null).getAbsolutePath ();
                  } catch (Exception e) {
                     Log.v ("Kanji", "No permission to access external storage, missing android.permission.WRITE_EXTERNAL_STORAGE");
                  }
               }
               return storagePath;
                  
            case 9:
               return ((Integer) android.os.Build.VERSION.SDK_INT).toString ();
               
            case 10:
               return getContext().getPackageName();
                  
            case 11:
               versionStr = "";
               try {
                  PackageInfo pkg = getContext().getPackageManager().getPackageInfo (getContext().getPackageName(), 0);
                  versionStr = pkg.versionName;
               } catch (Exception e) {                  
               }
               return versionStr;
                  
            case 12:
               versionStr = "";
               try {
                  PackageInfo pkg = getContext().getPackageManager().getPackageInfo (getContext().getPackageName(), 0);
                  versionStr = (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) ? ((Integer)(int)pkg.getLongVersionCode()).toString(): ((Integer) pkg.versionCode).toString();
               } catch (Exception e) {                  
               }
               return versionStr;
            
            case 13:
               obbPath = "";
               try {
                  String packageName = getContext().getPackageName();
                  PackageInfo pkg = getContext().getPackageManager().getPackageInfo (packageName, 0);
                  String versionCode = (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) ? ((Integer)(int)pkg.getLongVersionCode()).toString(): ((Integer) pkg.versionCode).toString();
                  
                  obbPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/obb/" + packageName + "/" +
                            "main." + versionCode + "." + packageName + ".obb";
               } catch (Exception e) {
               }
               return obbPath;
                                 
            case 14:
               obbPath = "";
               try {
                  String packageName = getContext().getPackageName();
                  PackageInfo pkg = getContext().getPackageManager().getPackageInfo (packageName, 0);
                  String versionCode = (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) ? ((Integer)(int)pkg.getLongVersionCode()).toString(): ((Integer) pkg.versionCode).toString();
                  
                  obbPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/obb/" + packageName + "/" +
                            "patch." + versionCode + "." + packageName + ".obb";
               } catch (Exception e) {
               }
               return obbPath;
            
            case 15:
               freeSpace = "0";
               try {
                  StatFs stat = new StatFs (Environment.getExternalStorageDirectory().getAbsolutePath());
                  long nAvailSize = (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) ? ((long) stat.getAvailableBlocksLong()) * ((long) stat.getBlockSizeLong()) :((long) stat.getAvailableBlocks()) * ((long) stat.getBlockSize());
                  int nMegs = (int) (nAvailSize / (1024*1024));
                  
                  freeSpace = ((Integer) nMegs).toString ();
               } catch (Exception e) {
               }
               return freeSpace;
                  
            case 16:
               memSize = "0";
               try {
                  ActivityManager activityManager = (ActivityManager) getContext().getSystemService (Context.ACTIVITY_SERVICE);
                  MemoryInfo memoryInfo = new ActivityManager.MemoryInfo();
                  
                  activityManager.getMemoryInfo (memoryInfo);
                  memSize = String.valueOf (memoryInfo.availMem);
               }
               catch (Exception e) {
               }
               return memSize;
                  
            default:
               return "";
            }
        }
       
        public String androidLaunchURL (String url) {
            Uri uri = Uri.parse (url);
            Intent intent = new Intent (Intent.ACTION_VIEW, uri);
         
            getContext().startActivity (intent);
            return null;
        }

        public String androidVibrate () {
            Vibrator v = (Vibrator) getContext().getSystemService (Context.VIBRATOR_SERVICE); 
            long[] pattern = { 0, 200, 500 };
            if (Build.VERSION.SDK_INT >=Build.VERSION_CODES.O) {
                v.vibrate(VibrationEffect.createOneShot(150,10));
            } else {
               v.vibrate(300);
            }
            return null;
        }

        public String androidShowMessageBox (String caption, String body) {
            final String finalCaption = caption;
            final String finalBody = body;
            final Semaphore semBlock = new Semaphore (0, true);

            mHandler.post (new Runnable () {
                public void run() { 
                    AlertDialog.Builder alertbox = new AlertDialog.Builder(getContext ());

                    alertbox.setTitle (finalCaption);
                    alertbox.setMessage (finalBody);
                    alertbox.setNeutralButton ("Ok", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface arg0, int arg1) {
                            semBlock.release ();
                        }
                    });
                   
                    alertbox.setOnCancelListener (new DialogInterface.OnCancelListener() {
                       public void onCancel (DialogInterface arg0) {
                          semBlock.release ();                         
                       }
                    });
                   
                    AlertDialog alertDialog = alertbox.create ();
                    alertDialog.setCanceledOnTouchOutside (false);
                    alertDialog.show ();
                }
            });

            try {
                boolean bAcquired;
               
                do {
                   bAcquired = semBlock.tryAcquire (100, TimeUnit.MILLISECONDS);
                   processEvents ();
                } while (!bAcquired);
            } catch (InterruptedException ex) {
            }

            return null;
        }

        String mAlertResult = "";

        public String androidShowAlertBox (String caption, String body, String button1Text, String button2Text) {
            final String finalCaption = caption;
            final String finalBody = body;
            final String finalButton1Text = button1Text;
            final String finalButton2Text = button2Text;
            final Semaphore semBlock = new Semaphore (0, true);

            mAlertResult = "";

            mHandler.post (new Runnable () {
                public void run() { 
                    AlertDialog.Builder alertbox = new AlertDialog.Builder(getContext ());

                    alertbox.setTitle (finalCaption);
                    alertbox.setMessage (finalBody);
                    alertbox.setPositiveButton (finalButton1Text, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface arg0, int arg1) {
                            mAlertResult = "0";
                            semBlock.release ();
                        }
                    });

                    if (finalButton2Text != "") {
                        alertbox.setNegativeButton (finalButton2Text, new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface arg0, int arg1) {
                                mAlertResult = "1";
                                semBlock.release ();
                            }
                        });
                    }

                    if (finalButton2Text != "") {
                       alertbox.setOnCancelListener (new DialogInterface.OnCancelListener() {
                          public void onCancel (DialogInterface arg0) {
                             mAlertResult = "1";
                             semBlock.release ();                         
                          }
                       });
                    }
                    else {
                       alertbox.setOnCancelListener (new DialogInterface.OnCancelListener() {
                          public void onCancel (DialogInterface arg0) {
                             mAlertResult = "0";
                             semBlock.release ();
                          }
                       });                       
                    }
                   
                    AlertDialog alertDialog = alertbox.create ();
                    alertDialog.setCanceledOnTouchOutside (false);
                    alertDialog.show ();
                }
            });

            try {
               boolean bAcquired;
               
               do {
                  bAcquired = semBlock.tryAcquire (100, TimeUnit.MILLISECONDS);
                  processEvents ();
               } while (!bAcquired);
            } catch (InterruptedException ex) {
            }

            return mAlertResult;
        }

        public String androidShowWebBrowser (String url, String closeButtonText) {
          final String finalUrl = url;
          final String finalCloseButtonText = closeButtonText;
          final Semaphore semBlock = new Semaphore (0, true);
          
          mHandler.post (new Runnable () {
             public void run() {
                AlertDialog.Builder alertbox = new AlertDialog.Builder(getContext ());
                WebView alertWebView = new WebView (getContext ());
                
                alertWebView.getSettings().setJavaScriptEnabled (true);
                alertWebView.getSettings().setUseWideViewPort (true);
                alertWebView.getSettings().setBuiltInZoomControls (true);
                alertWebView.getSettings().setLoadWithOverviewMode (true);
                alertWebView.getSettings().setDefaultZoom (ZoomDensity.FAR);
                alertWebView.loadUrl (finalUrl);
                alertWebView.setWebViewClient (new WebViewClient () {
                   @Override
                   public boolean shouldOverrideUrlLoading (WebView view, String url) {
                      view.loadUrl (url);
                      return true;
                   }                   
                });
                
                alertbox.setView (alertWebView);
                alertbox.setNegativeButton (finalCloseButtonText, new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface arg0, int arg1) {
                      semBlock.release ();
                   }
                });
                
                alertbox.setOnCancelListener (new DialogInterface.OnCancelListener() {
                   public void onCancel (DialogInterface arg0) {
                      semBlock.release ();
                   }
                });
                 
                AlertDialog alertDialog = alertbox.create ();
                alertDialog.setCanceledOnTouchOutside (false);
                
                WindowManager.LayoutParams lp = new WindowManager.LayoutParams();
                lp.width = WindowManager.LayoutParams.MATCH_PARENT;
                lp.height = WindowManager.LayoutParams.MATCH_PARENT;
                
                alertDialog.show ();
                alertDialog.getWindow().setAttributes (lp);
             }
          });
          
          try {
             boolean bAcquired;
             
             do {
                bAcquired = semBlock.tryAcquire (100, TimeUnit.MILLISECONDS);
                processEvents ();
             } while (!bAcquired);
          } catch (InterruptedException ex) {
          }
          
          return null;
        }
       
        public String androidShowBusyWaiting (final int nShow, final String strMessage) {
           mHandler.post (new Runnable() {
              public void run() {
                 if (nShow != 0) {
                    if (mProgressDialog == null) {
                       /* Show busy wheel */
                       mProgressDialogMessage = strMessage;
                       mProgressDialog = new ProgressDialog (getContext());
                       mProgressDialog.setMessage (mProgressDialogMessage);
                       mProgressDialog.setIndeterminate (true);
                       mProgressDialog.setCancelable (false);
                       mProgressDialog.show ();
                    }
                 }
                 else if (nShow == 0) {
                    if (mProgressDialog != null) {
                       /* Hide busy wheel */
                       mProgressDialog.dismiss ();
                       mProgressDialog = null;
                    }
                    mRestoreProgressDialog = false;
                 }
              }
           });
           
           return null;
        }
       
        public String androidShowToast (final String strMessage, final int nLong) {
           mHandler.post (new Runnable() {
              public void run() {
                 Toast toastInstance = Toast.makeText (getContext(), strMessage, (nLong != 0) ? Toast.LENGTH_LONG : Toast.LENGTH_SHORT);
                 
                 toastInstance.show ();
              }
           });
          
           return null;
        }
       
        public String androidSendEmail (String toList, String ccList, String bccList, String emailSubject, String emailBody) {
           String toArray[] = toList.split (";");
           String ccArray[] = ccList.split (";");
           String bccArray[] = bccList.split (";");
           final Intent emailIntent = new Intent (android.content.Intent.ACTION_SEND);
           int i;
          
           emailIntent.setType ("text/rfc822");
          
           if (toList.length() > 0 && toArray.length > 0) emailIntent.putExtra (android.content.Intent.EXTRA_EMAIL, toArray);
           if (ccList.length() > 0 && ccArray.length > 0) emailIntent.putExtra (android.content.Intent.EXTRA_CC, ccArray);
           if (bccList.length() > 0 && bccArray.length > 0) emailIntent.putExtra (android.content.Intent.EXTRA_BCC, bccArray);
          
           emailIntent.putExtra (android.content.Intent.EXTRA_SUBJECT, emailSubject);
           emailIntent.putExtra (android.content.Intent.EXTRA_TEXT, Html.fromHtml (emailBody));
           getContext().startActivity (Intent.createChooser (emailIntent, "Send email"));
          
           return "0";
        }
       
        public Integer androidGetMinSoundBufferSize (int nSampleRate, int nChannels, int nBitsPerSample) {
           return (Integer) KanjiSound.getMinSoundBufferSize (nSampleRate, nChannels, nBitsPerSample);
        }
        
        public KanjiSound androidCreateSound (int nSampleRate, int nChannels, int nBitsPerSample, int nBufferSize, boolean bLoops, boolean bStatic) {
            return new KanjiSound (nSampleRate, nChannels, nBitsPerSample, nBufferSize, bLoops, bStatic);
        }

        public KanjiMediaPlayer androidCreateMediaPlayer (String path) {
            return new KanjiMediaPlayer (getContext(), path);
        }

        public KanjiMoviePlayer androidCreateMoviePlayer (String path, int x1, int y1, int x2, int y2, 
                                                          boolean bVideoLoops, float fLeftVolume, float fRightVolume, boolean bPlayBehindKWindow) {
            return new KanjiMoviePlayer (getContext(), path, x1, y1, x2, y2, bVideoLoops, fLeftVolume, fRightVolume, bPlayBehindKWindow);
        }
       
        public String androidIsActivityPaused() {
            if (mPaused)
               return "1";
            else
               return "0";
        }
       
        /**
         * Check if the Internet is currently reachable
         *
         * \return 1 if it is reachable, 0 if not
         */
        
        public Integer androidIsInternetReachable () {
           String hostAddr = null;
           
           try {
              for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); hostAddr == null && en.hasMoreElements();) {
                 NetworkInterface intf = en.nextElement();
                 
                 for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    
                    if (inetAddress instanceof Inet4Address) {
                       if (!inetAddress.isLoopbackAddress()) {
                          byte [] ba = inetAddress.getAddress();
                          hostAddr = Arrays.toString (ba);
                       }
                    }
                 }
              }
           } catch (SocketException ex) {
              Log.v ("Kanji", "androidIsInternetReachable: need permissions android.permission.ACCESS_NETWORK_STATE and android.permission.INTERNET");
           }
           
           return (hostAddr != null && hostAddr.length() > 0) ? 1 : 0;
        }
        
        /**
         * Perform http request for the specified URL, then download the response asynchronously, using HTTP GET
         *
         * \param nHttpRequestId identifier for the http request
         * \param strURL URL to do http get for
         */
        
        public void androidDoHttpGet (final int nHttpRequestId, final String strURL, final String strResponseFilePath, final int nUseResponseFile, final boolean bypassCache) {
           Thread thread = new Thread () {
              @Override
              public void run() {
                 HttpURLConnection connection = null;
                 BufferedInputStream inReader = null;
                 byte cbArray[] = new byte [131072];
                 int nReadBytes;
                 
                 try {
                    URL serverAddress = new URL (strURL);
                    
                    connection = (HttpURLConnection) serverAddress.openConnection ();
                    connection.setRequestMethod ("GET");
                    if (bypassCache) {
                       connection.setUseCaches(false);
                    }
                    connection.setReadTimeout (10000);
                    connection.connect ();
                    
                    inReader = new BufferedInputStream (connection.getInputStream ());
                    
                    if (nUseResponseFile != 0) {
                       FileOutputStream responseWriter = new FileOutputStream (strResponseFilePath, false);
                       
                       do {
                          nReadBytes = inReader.read (cbArray);
                          if (nReadBytes > 0) {
                             responseWriter.write (cbArray, 0, nReadBytes);
                          }
                       } while (nReadBytes > 0);
                       
                       final Integer responseCode = connection.getResponseCode ();
                       responseWriter.close ();
                       responseWriter = null;
                       inReader.close ();
                       
                       setEvent (new Runnable() {
                          public void run() {
                             /* Post successful http request event to the game */
                             
                             mHttpRequestSuccess = true;
                             mHttpRequestId = nHttpRequestId;
                             mHttpRequestResponseCode = (Integer) responseCode;
                             mHttpRequestResponseData = null;
                             
                             KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                             
                             mHttpRequestId = 0;
                             mHttpRequestResponseCode = 0;
                             mHttpRequestResponseData = null;
                          }
                       });
                    }
                    else {
                       ByteArrayOutputStream responseWriter = new ByteArrayOutputStream ();
                       
                       do {
                          nReadBytes = inReader.read (cbArray);
                          if (nReadBytes > 0) {
                             responseWriter.write (cbArray, 0, nReadBytes);
                          }
                       } while (nReadBytes > 0);
                                           
                       final byte [] downloadedData = responseWriter.toByteArray();
                       final Integer responseCode = connection.getResponseCode ();
                       responseWriter.close ();
                       responseWriter = null;
                       inReader.close ();
                       
                       setEvent (new Runnable() {
                           public void run() {
                              /* Post successful http request event to the game */
                              
                              mHttpRequestSuccess = true;
                              mHttpRequestId = nHttpRequestId;
                              mHttpRequestResponseCode = (Integer) responseCode;
                              mHttpRequestResponseData = downloadedData;
                              
                              KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                              
                              mHttpRequestId = 0;
                              mHttpRequestResponseCode = 0;
                              mHttpRequestResponseData = null;
                           }
                       });
                    }
                 } catch (Exception e) {
                    Log.v ("Kanji", "exception during http get request: " + e.toString());
                    
                    setEvent (new Runnable() {
                        public void run() {
                           /* Post http request error event */
                           
                           mHttpRequestSuccess = false;
                           mHttpRequestId = nHttpRequestId;
                           mHttpRequestResponseCode = 0;
                           
                           KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                           
                           mHttpRequestId = 0;
                           mHttpRequestResponseCode = 0;
                        }
                     });
                 }
                 finally {
                    if (connection != null)
                       connection.disconnect ();
                    inReader = null;
                    connection = null;
                    cbArray = null;
                 }
              }
           };
           
           thread.start ();
        }
        
        /**
         * Perform http request and post data to the specified URL, then download the response asynchronously, using HTTP POST
         *
         * \param nHttpRequestId identifier for the http request
         * \param strURL URL to do http post for
         * \param payloadData payload data to post
         */
        
        public void androidDoHttpPost (final int nHttpRequestId, final String strURL, final byte[] payloadData,
                                       final String strResponseFilePath, final int nUseResponseFile, final boolean bypassCache) {
           Thread thread = new Thread () {
              @Override
              public void run() {
                 HttpURLConnection connection = null;
                 BufferedInputStream responseReader = null;
                 DataOutputStream payloadWriter = null;
                 byte cbArray[] = new byte [131072];
                 int nReadBytes;
                 
                 try {
                    URL serverAddress = new URL (strURL);
                    
                    connection = (HttpURLConnection) serverAddress.openConnection ();
                    connection.setRequestMethod ("POST");
                    if (bypassCache) {
                       connection.setUseCaches (false);
                    }
                    connection.setDoInput (true);
                    connection.setDoOutput (true);
                    connection.setReadTimeout (10000);
                    connection.setConnectTimeout (15000);
                    connection.setRequestProperty ("Content-Length", Integer.toString (payloadData.length));
                    connection.setRequestProperty ("Content-Type", "application/x-www-form-urlencoded");
                    connection.connect ();
                    
                    payloadWriter = new DataOutputStream (connection.getOutputStream ());
                    payloadWriter.write (payloadData);
                    payloadWriter.flush ();
                    payloadWriter.close ();
                    
                    responseReader = new BufferedInputStream (connection.getInputStream ());
                    
                    if (nUseResponseFile != 0) {
                       FileOutputStream responseWriter = new FileOutputStream (strResponseFilePath, false);
                       
                       do {
                          nReadBytes = responseReader.read (cbArray);
                          if (nReadBytes > 0) {
                             responseWriter.write (cbArray, 0, nReadBytes);
                          }
                       } while (nReadBytes > 0);
                       
                       final Integer responseCode = connection.getResponseCode ();
                       responseWriter.close ();
                       responseWriter = null;
                       responseReader.close ();
                       
                       setEvent (new Runnable() {
                          public void run() {
                             /* Post successful http request event to the game */
                             
                             mHttpRequestSuccess = true;
                             mHttpRequestId = nHttpRequestId;
                             mHttpRequestResponseCode = (Integer) responseCode;
                             mHttpRequestResponseData = null;
                             
                             KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                             
                             mHttpRequestId = 0;
                             mHttpRequestResponseCode = 0;
                             mHttpRequestResponseData = null;
                          }
                       });
                    }
                    else {
                       ByteArrayOutputStream responseWriter = new ByteArrayOutputStream ();
                       
                       do {
                          nReadBytes = responseReader.read (cbArray);
                          if (nReadBytes > 0) {
                             responseWriter.write (cbArray, 0, nReadBytes);
                          }
                       } while (nReadBytes > 0);
                       
                       final byte [] downloadedData = responseWriter.toByteArray();
                       final Integer responseCode = connection.getResponseCode ();
                       responseWriter.close ();
                       responseWriter = null;
                       responseReader.close ();
                       
                       setEvent (new Runnable() {
                           public void run() {
                              /* Post successful http request event to the game */
                              
                              mHttpRequestSuccess = true;
                              mHttpRequestId = nHttpRequestId;
                              mHttpRequestResponseCode = (Integer) responseCode;
                              mHttpRequestResponseData = downloadedData;
                              
                              KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                              
                              mHttpRequestId = 0;
                              mHttpRequestResponseCode = 0;
                              mHttpRequestResponseData = null;
                           }
                       });
                    }
                 } catch (Exception e) {
                     Log.v ("Kanji", "exception during http post request: " + e.toString());
                     
                     setEvent (new Runnable() {
                         public void run() {
                            /* Post http request error event */
                            
                            mHttpRequestSuccess = false;
                            mHttpRequestId = nHttpRequestId;
                            mHttpRequestResponseCode = 0;
                            
                            KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                            
                            mHttpRequestId = 0;
                            mHttpRequestResponseCode = 0;
                         }
                     });
                 }
                 finally {
                    if (connection != null)
                       connection.disconnect ();
                    responseReader = null;
                    payloadWriter = null;
                    connection = null;
                    cbArray = null;
                 }
              }
           };
           
           thread.start ();
        }
        
        /**
         * Execute a http delete request for the specified URL
         *
         * \param nHttpRequestId identifier for the http request
         * \param strURL URL to http delete for
         */
        
        public void androidDoHttpDelete (final int nHttpRequestId, final String strURL, final String strResponseFilePath, final int nUseResponseFile, final boolean bypassCache) {
           Thread thread = new Thread () {
              @Override
              public void run() {
                 HttpURLConnection connection = null;
                 BufferedInputStream responseReader = null;
                 DataOutputStream payloadWriter = null;
                 byte cbArray[] = new byte [131072];
                 int nReadBytes;
                 
                 try {
                    URL serverAddress = new URL (strURL);
                    
                    connection = (HttpURLConnection) serverAddress.openConnection ();
                    connection.setRequestMethod ("DELETE");
                    if (bypassCache) {
                       connection.setUseCaches (false);
                    }
                    connection.connect ();
                    
                    responseReader = new BufferedInputStream (connection.getInputStream ());
                    
                    if (nUseResponseFile != 0) {
                       FileOutputStream responseWriter = new FileOutputStream (strResponseFilePath, false);
                       
                       do {
                          nReadBytes = responseReader.read (cbArray);
                          if (nReadBytes > 0) {
                             responseWriter.write (cbArray, 0, nReadBytes);
                          }
                       } while (nReadBytes > 0);
                       
                       final Integer responseCode = connection.getResponseCode ();
                       responseWriter.close ();
                       responseWriter = null;
                       responseReader.close ();
                       
                       setEvent (new Runnable() {
                          public void run() {
                             /* Post successful http request event to the game */
                             
                             mHttpRequestSuccess = true;
                             mHttpRequestId = nHttpRequestId;
                             mHttpRequestResponseCode = (Integer) responseCode;
                             mHttpRequestResponseData = null;
                             
                             KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                             
                             mHttpRequestId = 0;
                             mHttpRequestResponseCode = 0;
                             mHttpRequestResponseData = null;
                          }
                       });
                    }
                    else {
                       ByteArrayOutputStream responseWriter = new ByteArrayOutputStream ();
                       
                       do {
                          nReadBytes = responseReader.read (cbArray);
                          if (nReadBytes > 0) {
                             responseWriter.write (cbArray, 0, nReadBytes);
                          }
                       } while (nReadBytes > 0);
                       
                       final byte [] downloadedData = responseWriter.toByteArray();
                       final Integer responseCode = connection.getResponseCode ();
                       responseWriter.close ();
                       responseWriter = null;
                       responseReader.close ();
                       
                       setEvent (new Runnable() {
                           public void run() {
                              /* Post successful http request event to the game */
                              
                              mHttpRequestSuccess = true;
                              mHttpRequestId = nHttpRequestId;
                              mHttpRequestResponseCode = (Integer) responseCode;
                              mHttpRequestResponseData = downloadedData;
                              
                              KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                              
                              mHttpRequestId = 0;
                              mHttpRequestResponseCode = 0;
                              mHttpRequestResponseData = null;
                           }
                       });
                    }
                 } catch (Exception e) {
                     Log.v ("Kanji", "exception during http delete request: " + e.toString());
                     
                     setEvent (new Runnable() {
                         public void run() {
                            /* Post http request error event */
                            
                            mHttpRequestSuccess = false;
                            mHttpRequestId = nHttpRequestId;
                            mHttpRequestResponseCode = 0;
                            
                            KanjiGameLib.handleUserEvent (getGLThread(), 122 /* K_EVENT_HTTP_REQUEST */);
                            
                            mHttpRequestId = 0;
                            mHttpRequestResponseCode = 0;
                         }
                     });
                 }
                 finally {
                    if (connection != null)
                       connection.disconnect ();
                    connection = null;
                 }
              }
           };
           
           thread.start ();
        }
       
        public boolean androidCheckPermissions (String permissions) {
           String [] permissionsArray;
           
           if (permissions != null)
              permissionsArray = permissions.split(";");
           else
              permissionsArray = new String []{};
           
           // Check each permission
           for (int i = 0; i < permissionsArray.length; i++) {
               if (ContextCompat.checkSelfPermission((Activity) getContext(), permissionsArray[i]) != PackageManager.PERMISSION_GRANTED)
                  return false;
           }
           
           // All permissions granted
           return true;
        }
       
        public boolean androidRequestPermissions (String permissions, final String rationale) {
           String [] permissionsArray;
           int nMissingPermissionIdx = -1;
           
           if (permissions != null)
              permissionsArray = permissions.split(";");
           else
              permissionsArray = new String []{};
           
           for (int i = 0; nMissingPermissionIdx < 0 && i < permissionsArray.length; i++) {
              if (ContextCompat.checkSelfPermission((Activity) getContext(), permissionsArray[i]) != PackageManager.PERMISSION_GRANTED) {
                 nMissingPermissionIdx = i;
                 break;
              }
           }
           
           if (nMissingPermissionIdx < 0) {
              // Permissions already granted
              return true;
           }
           
           mSemBlockPermissions = new Semaphore (0, true);
           mRequestPermissionsResult = false;
           
           final String[] finalPermissionsArray = permissionsArray;
           if (ActivityCompat.shouldShowRequestPermissionRationale((Activity) getContext(),permissionsArray[nMissingPermissionIdx])) {
              // Show rationale for requesting the permissions
              Log.d("Kanji", "show permissions rationale");
              mHandler.post (new Runnable () {
                 public void run() {
                    AlertDialog.Builder alertbox = new AlertDialog.Builder(getContext ());
                    
                    alertbox.setMessage (rationale);
                    alertbox.setNeutralButton ("Ok", new DialogInterface.OnClickListener() {
                       public void onClick(DialogInterface arg0, int arg1) {
                          // Try requesting the permissions now
                          Log.d("Kanji", "request permissions now");
                          ActivityCompat.requestPermissions((Activity) getContext(), finalPermissionsArray, 100);
                       }
                    });
                    
                    alertbox.setOnCancelListener (new DialogInterface.OnCancelListener() {
                       public void onCancel (DialogInterface arg0) {
                          // Try requesting the permissions now
                          Log.d("Kanji", "request permissions now");
                          ActivityCompat.requestPermissions((Activity) getContext(), finalPermissionsArray, 100);
                       }
                    });
                    
                    AlertDialog alertDialog = alertbox.create ();
                    alertDialog.setCanceledOnTouchOutside (false);
                    alertDialog.show ();
                 }
              });
           }
           else {
              mHandler.post (new Runnable () {
                 public void run() {
                    // Directly request the permissions
                    Log.d("Kanji", "request permissions directly");
                    ActivityCompat.requestPermissions((Activity) getContext(), finalPermissionsArray, 100);
                 }
              });
           }
           
           // Wait for the outcome
           try {
              boolean bAcquired;
              
              do {
                 bAcquired = mSemBlockPermissions.tryAcquire (100, TimeUnit.MILLISECONDS);
                 processEvents ();
              } while (!bAcquired);
           } catch (InterruptedException ex) {
           }
           
           mSemBlockPermissions = null;
           return mRequestPermissionsResult;
        }
       
        public Integer androidGetCurHttpRequestSuccess() {
        	return mHttpRequestSuccess ? 1 : 0;
        }
        
        public Integer androidGetCurHttpRequestId () {
           return mHttpRequestId;
        }
        
        public Integer androidGetCurHttpRequestResponseCode () {
            return mHttpRequestResponseCode;
        }
        
        public byte[] androidGetCurHttpRequestResponseData() {
           return mHttpRequestResponseData;
        }
       
        public void androidScheduleLocalNotification (int nDaysFromNow, int nHours, int nMinutes, int nSeconds,
                                                      int nIsAbsoluteTime,
                                                      String message, int nUserData) {
			try {
				Context context = getContext();
            AlarmManager am = (AlarmManager) context.getSystemService (Activity.ALARM_SERVICE);
            Calendar cal = Calendar.getInstance ();
            PackageInfo pkg = context.getPackageManager().getPackageInfo (getContext().getPackageName(), 0);
				String strAppName = "Notification";
            int nIconResId = 0;
				
				try {
					int nAppNameStringId = context.getApplicationInfo().labelRes;
					strAppName = context.getString (nAppNameStringId);
				} catch (Exception e) {
				}
            
				try {
					nIconResId = context.getApplicationInfo().icon;
				} catch (Exception e) {
				}
            
            cal.add (Calendar.DAY_OF_MONTH, nDaysFromNow);
            if (nIsAbsoluteTime == 0) {
               cal.add (Calendar.HOUR_OF_DAY, nHours);
               cal.add (Calendar.MINUTE, nMinutes);
               cal.add (Calendar.SECOND, nSeconds);
            }
            else {
               cal.set (Calendar.HOUR_OF_DAY, nHours);
               cal.set (Calendar.MINUTE, nMinutes);
               cal.set (Calendar.SECOND, nSeconds);
            }
            
            Intent intent = new Intent(context, KanjiNotificationReceiver.class);
            intent.putExtra ("k_title", strAppName);
            intent.putExtra ("k_message", message);
            intent.putExtra ("k_package", context.getApplicationContext().getPackageName());
            intent.putExtra ("k_activity", ((Activity) context).getLocalClassName());
            intent.putExtra ("k_iconresource", nIconResId);
            intent.putExtra ("k_userdata", nUserData);
            PendingIntent sender = PendingIntent.getBroadcast(context, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);
            
            am.cancel (sender);
            am.set (AlarmManager.RTC_WAKEUP, cal.getTimeInMillis(), sender);
			}
			catch (Exception e) {
				Log.v ("Kanji", "exception in scheduleLocalNotification: " + e.toString());
			}	
		 }
		
		 public void androidCancelLocalNotifications() {
			 try {
				Context context = getContext();
            AlarmManager am = (AlarmManager) context.getSystemService (Activity.ALARM_SERVICE);
            
            am.cancel (PendingIntent.getBroadcast(context, 0, new Intent (context, KanjiNotificationReceiver.class), PendingIntent.FLAG_UPDATE_CURRENT));
			 }
			 catch(Exception e) {
 		 		Log.v ("Kanji", "exception in cancelLocalNotifications: " + e.toString());
	 		 }
		  }

        public Integer androidGetLocalNotificationUserData() {
           return mLocalNotificationUserData;
        }
       
        private boolean needToWait() {
            return (mPaused || (! mHasFocus) || (! mHasSurface) || mContextLost)
                && (! mDone) && (mProgressDialog == null);
        }

        public void surfaceCreated() {
            synchronized(this) {
                mHasSurface = true;
                mContextLost = false;
                notify();
            }
        }

        public void surfaceDestroyed() {
            synchronized(this) {
                mHasSurface = false;
                notify();
            }
        }

        public void onPause() {
            synchronized (this) {
                mPaused = true;
                mNotifyPause = true;
                mNotifyResume = false;
                notify();
            }
        }

        public void onResume() {
            synchronized (this) {
                mPaused = false;
                mNotifyPause = false;
                mNotifyResume = true;
                notify();
            }
        }

        public void onWindowFocusChanged(boolean hasFocus) {
            synchronized (this) {
                mHasFocus = hasFocus;
                if (mHasFocus == true) {
                    notify();
                }
            }
        }
        public void onWindowResize(int w, int h) {
            synchronized (this) {
                mWidth = w;
                mHeight = h;
                mSizeChanged = true;
            }
        }

        public void requestExitAndWait() {
            // don't call this from GLThread thread or it is a guaranteed
            // deadlock!
            synchronized(this) {
                mDone = true;
                notify();
            }
            try {
                join();
            } catch (InterruptedException ex) {
                Thread.currentThread().interrupt();
            }
        }

        /**
         * Queue an "event" to be run on the GL rendering thread.
         * @param r the runnable to be run on the GL rendering thread.
         */
        public void setEvent(final Runnable r) {
            synchronized(this) {
                try {
                    mEventQueue.add (r);
                } catch (Exception e) {
                }
            }
        }
        
       public void clearEvent() {
          synchronized(this) {
             mEventQueue.clear ();
          }
       }



        // Sac: Added by me to set Acheivements & In App Purchase of parent Activity from this thread.
        public void SetAccomplishments(int Award_id) { //This function calls the same function of Main Activity to set accomplisments like Aceivements in Google Play stpre

            /*
            if(getContext() instanceof KanjiActivity) {
                KanjiActivity activity = (KanjiActivity) getContext();
                activity.SetAccomplishments(Award_id);
            }
            */

            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.SetAccomplishments(Award_id);
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in SetAccomplishments: " + e.toString());
            }
        }
        public void UpgradeAppButtonClicked(){ // Override this function in Main Activity to write the code to call In App billing of Goolge.
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.UpgradeAppButtonClicked();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in UpgradeAppButtonClicked: " + e.toString());
            }
        }

        public void AnalyticsSkipPuzzle(int puzzleID){ // Override this function in Main Activity
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.AnalyticsSkipPuzzle(puzzleID);
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in AnalyticsSkipPuzzle: " + e.toString());
            }
        }

        public void UpdateUserHint(String data)
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.UpdateUserHint(data);
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in UpdateUserHint: " + e.toString());
            }
        }

        public void showInterstitial()
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.showInterstitial();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in showInterstitial: " + e.toString());
            }
        }

        public void showRewardedAD()
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.showRewardedAD();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in showRewardedAD: " + e.toString());
            }
        }

        public void ShowHintStore()
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.ShowHintStore();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in ShowHintStore: " + e.toString());
            }
        }

        public void ShowUnlockGame()
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.ShowUnlockGame();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in ShowUnlockGame: " + e.toString());
            }
        }

        public void ShowFullAssetDownloader()
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.ShowFullAssetDownloader();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in ShowFullAssetDownloader: " + e.toString());
            }
        }

        public void FetchUserData()
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.FetchUserData();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in FetchUserData: " + e.toString());
            }
        }

        public void SignInToAccount()
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.SignInToAccount();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in SignInToAccount: " + e.toString());
            }
        }

        public void SignOutAccount()
        {
            try {
                Context context = getContext();
                if(context instanceof KanjiActivity) {
                    KanjiActivity activity = (KanjiActivity) context;
                    activity.SignOutAccount();
                }
            }
            catch(Exception e) {
                Log.v ("Kanji", "exception in SignOutAccount: " + e.toString());
            }
        }
    }
   
   private static final Semaphore sEglSemaphore = new Semaphore(1);
   private static boolean mSizeChanged = true;
   
   private static boolean mInitialized = false;
   private static SurfaceHolder mHolder = null;
   private static GLThread mGLThread = null;
   private static GLWrapper mGLWrapper = null;
   private static KanjiGLSurfaceView mView = null;
   private static InputMethodManager mIme = null;
   
   private static AssetManager mAssetManager = null;
   private static Handler mHandler = null;
   private static boolean mDone = false;
   private static boolean mPaused = false;
   private static boolean mNotifyPause = false;
   private static boolean mNotifyResume = false;
   private static boolean mNeedOrientationFix = false;
   private static boolean mHasFocus = false;
   private static boolean mHasSurface = false;
   private static boolean mContextLost = false;
   private static boolean mVirtualKeyboardShown = false;
   private static boolean mVirtualKeyboardDidShow = false;
   private static boolean mRestoreKeyboard = false;
   private static boolean mCatchBackKey = false;
   private static boolean mBackbufferHasAlpha = false;
   private static boolean mPreserveEGLContextOnPause = false;
   private static boolean mReloadNeeded = false;
   private static boolean mFocusLost = false;
   private static boolean mDeliverNativeEvents = false;
   private static int mWidth = 0;
   private static int mHeight = 0;
   private static int mRotation = 0;
   private static Queue<Runnable> mEventQueue = new LinkedList<Runnable> ();
   private static EglHelper mEglHelper;
   private static GL10 mGl = null;
   private static boolean mTellRendererSurfaceCreated = false;
   private static boolean mTellRendererSurfaceChanged = false;
   private static int mCurWidth = 0;
   private static int mCurHeight = 0;
   private static ProgressDialog mProgressDialog = null;
   private static String mProgressDialogMessage = "";
   private static boolean mRestoreProgressDialog = false;
   private static boolean mHttpRequestSuccess = false;
   private static Integer mHttpRequestId = 0;
   private static Integer mHttpRequestResponseCode = 0;
   private static byte[] mHttpRequestResponseData = null;
   private static int mLocalNotificationUserData = 0;
   private static Semaphore mSemBlockPermissions;
   private static boolean mRequestPermissionsResult;
}
