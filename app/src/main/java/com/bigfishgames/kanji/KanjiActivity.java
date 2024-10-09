package com.bigfishgames.kanji;

import android.os.Bundle;
import android.graphics.Rect;
import android.view.View;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;
import android.view.WindowManager;
import android.view.OrientationEventListener;
import android.view.View.OnSystemUiVisibilityChangeListener;
import android.widget.RelativeLayout;
import android.widget.FrameLayout;
import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.os.Handler;
import android.util.Log;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import androidx.core.app.ActivityCompat;
import androidx.appcompat.app.AppCompatActivity;

public class KanjiActivity extends AppCompatActivity implements SensorEventListener, ActivityCompat.OnRequestPermissionsResultCallback
{
    /* Intents receiver */
    public class KanjiIntentReceiver extends BroadcastReceiver {
       @Override
       public void onReceive (final Context context, final Intent intent) {
          if (intent != null && intent.getAction().equals (Intent.ACTION_DREAMING_STARTED)) {
             /* Dream mode started */
             if (view != null) {
                /* Send K_EVENT_SCREENSAVER_START */
                view.onUserEvent (119);
             }
          }
          if (intent != null && intent.getAction().equals (Intent.ACTION_DREAMING_STOPPED)) {
             /* Dream mode stopped */
             if (view != null) {
                /* Send K_EVENT_SCREENSAVER_STOP */
                view.onUserEvent (120);
             }
          }
       }
    }
   
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        this.requestWindowFeature (Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                             WindowManager.LayoutParams.FLAG_FULLSCREEN);
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);

        KanjiGameLib.initWithAssetManager(getAssets());

        FrameLayout mainLayout = (FrameLayout)findViewById(android.R.id.content);

        RelativeLayout relativeLayout = new RelativeLayout(this);
        view = new KanjiGLSurfaceView (this, getAssets(), new Handler ());
        view.setContentDescription("Game Main View");
        view.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO_HIDE_DESCENDANTS);
        relativeLayout.addView(view);
       if(mainLayout!=null)
           mainLayout.addView(relativeLayout);
           else
            setContentView(relativeLayout,
                       new RelativeLayout.LayoutParams (RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT)); // FILL_PARENT is deprecated from API 16, using MATCH_PARENT
        mOrientationEventListener = null;
       
        if (android.os.Build.VERSION.SDK_INT > 8) {
           String strValue;
           
           try {
              ApplicationInfo appInfo = getPackageManager().getApplicationInfo (getPackageName(), PackageManager.GET_META_DATA);
              Object value = appInfo.metaData.get ("kanji-portrait");
              
              strValue = value.toString ();
           }
           catch (Exception e) {
              strValue = "";
           }
           
           if (strValue.equals ("yes")) {
              Log.v ("Kanji", "Requesting sensor portrait orientation");
              setRequestedOrientation (ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);   /* sensorPortrait */
           }
           else {
              Log.v ("Kanji", "Requesting sensor landscape orientation");
              setRequestedOrientation (ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);   /* sensorLandscape */
           }
        }
       
        if (android.os.Build.VERSION.SDK_INT >= 19) {
          String strValue;
          
          try {
             ApplicationInfo appInfo = getPackageManager().getApplicationInfo (getPackageName(), PackageManager.GET_META_DATA);
             Object value = appInfo.metaData.get ("kanji-immersive");
             
             strValue = value.toString ();
             
             if (strValue.equals ("yes")) {
                Log.v ("Kanji", "Enable immersive mode");
                mImmersiveMode = true;
                
                /* Set immersive mode when the system UI changes visibility */
                View decorView = getWindow().getDecorView();
                decorView.setOnSystemUiVisibilityChangeListener(new OnSystemUiVisibilityChangeListener() {
                   @Override
                   public void onSystemUiVisibilityChange(int visibility) {
                      if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                         setImmersiveMode();
                      }
                   }
                });
             }
          }
          catch (Exception e) {
             strValue = "";
          }
        }
       
        /* Observe root view's layout changes to know when the virtual keyboard is showing/hiding */
        final View activityRootView = findViewById (android.R.id.content);
        activityRootView.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO_HIDE_DESCENDANTS); // Sac: This is to fix the warning in Play store which says that the Views in app has to have a label readable by screen readers.
        activityRootView.getViewTreeObserver().addOnGlobalLayoutListener (new OnGlobalLayoutListener() {
           @Override
           public void onGlobalLayout () {
              Rect r = new Rect ();
              
              activityRootView.getWindowVisibleDisplayFrame (r);
              int heightDiff = activityRootView.getRootView().getHeight() - (r.bottom-r.top);
              
              if (heightDiff < 100)
                 view.onVirtualKeyboardHidden();
              else
                 view.onVirtualKeyboardShown();
           }
        });
       
        /* Start listening for the Android 4.2 dream state starting/stopping intents */
       
        final IntentFilter filter = new IntentFilter();
        filter.addAction (Intent.ACTION_DREAMING_STARTED);
        filter.addAction (Intent.ACTION_DREAMING_STOPPED);
        mKanjiIntentReceiver = new KanjiIntentReceiver ();
        registerReceiver (mKanjiIntentReceiver, filter);
    }
   
    @Override
    protected void onDestroy () {
       if (mKanjiIntentReceiver != null) {
          /* Stop listening for intents */
          unregisterReceiver (mKanjiIntentReceiver);
          mKanjiIntentReceiver = null;
       }
       
       super.onDestroy ();
       Log.v ("Kanji", "Exit");
       System.exit(0);
    }
   
    @Override
    protected void onPause () {
        super.onPause ();
        view.onPause ();
        KanjiSound.onPause ();
		  sensorManager.unregisterListener (this, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER));
        if (mOrientationEventListener != null)
           mOrientationEventListener.disable ();
    }
    
    @Override
    protected void onResume () {
        Intent intent;
       
        super.onResume ();
        view.onResume ();
        KanjiSound.onResume ();
        sensorManager.registerListener (this, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_GAME);
        if (mOrientationEventListener != null) {
           if (mOrientationEventListener.canDetectOrientation ())
              mOrientationEventListener.enable ();
        }
       
        intent = getIntent ();
        if (intent != null && intent.getIntExtra ("k_is_notification", 0) != 0) {
           int nUserData = intent.getIntExtra ("k_userdata", 0);
           
           /* Tell the game that we received a local notification */
           Log.v ("kanji", "onResume: received local notification");
           view.onLocalNotificationEvent (nUserData);
           
           /* Clear intent's extras so that this notification isn't fired again */
           intent.replaceExtras ((Bundle) null);
        }
    }
    
    @Override
    public void onConfigurationChanged (Configuration newConfig) {
        if (newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_YES) {
           view.onUserEvent (113); /* K_EVENT_KEYBOARD_DISMISS */
        }
        super.onConfigurationChanged (newConfig);
    }

    @Override
    public void onWindowFocusChanged (boolean hasFocus) {
       try {
          if (hasFocus) {
             /* Set immersive mode */
             setImmersiveMode();
          }
          
          if (hasFocus == true)
             view.getGLThread().onFocusLost ();
       }
       catch (Exception e) {
       }
       
       super.onWindowFocusChanged (hasFocus);
    }
   
    @Override
	 public void onSensorChanged(SensorEvent event) {
		  synchronized (this) {
			   switch (event.sensor.getType()) {
			   case Sensor.TYPE_ACCELEROMETER:
				    view.onAccelerometer (event.values[0], event.values[1], event.values[2]);
                if (view.needOrientationFixNow ()) {
                    fixOrientation ();
                    view.orientationFixed ();
                }
				    break;

			   default:
				    break;
			   }           
		  }
	 }

    @Override
	 public void onAccuracyChanged(Sensor sensor, int accuracy) {
	 }
   
    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
       if (view != null)
          view.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    private void fixOrientation () {
        if (android.os.Build.DEVICE.equals ("SGH-T959")) {
           /* Work around a bug that shows up at least on the Samsung Galaxy S when coming out of device sleep */
           setRequestedOrientation (ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
           setRequestedOrientation (ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        }
    }
   
    private void setImmersiveMode() {
       try {
         if (mImmersiveMode) {
            int uiOptions = getWindow().getDecorView().getSystemUiVisibility();
            
            if (android.os.Build.VERSION.SDK_INT >= 14) {
               uiOptions |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
            }
            
            if (android.os.Build.VERSION.SDK_INT >= 16) {
               uiOptions |= View.SYSTEM_UI_FLAG_FULLSCREEN;
               uiOptions |= View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
               uiOptions |= View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
            }
            
            if (android.os.Build.VERSION.SDK_INT >= 19) {
               uiOptions |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            }
            
            getWindow().getDecorView().setSystemUiVisibility(uiOptions);
         }
       } catch (Throwable e) {
         Log.e("SDL", "exception in setImmersiveMode: " + e.toString(), e);
       }
    }

    public void SetAccomplishments(int Award_id) {}

    public void UpgradeAppButtonClicked() {}

    public void AnalyticsSkipPuzzle(int puzzleID) {}

    public void UpdateUserHint(String data){}

    public void showInterstitial() {}
    public void showRewardedAD(){}

    public void ShowHintStore(){}
    public void ShowUnlockGame(){}
    public void ShowFullAssetDownloader(){}

    public void FetchUserData(){}

    public void SignInToAccount(){}
    public void SignOutAccount(){}

    public KanjiGLSurfaceView view;
    private boolean mImmersiveMode = false;
	 private SensorManager sensorManager;
    private OrientationEventListener mOrientationEventListener = null;
    private BroadcastReceiver mKanjiIntentReceiver = null;
}
