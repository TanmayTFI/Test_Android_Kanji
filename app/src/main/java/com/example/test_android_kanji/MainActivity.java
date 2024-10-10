package com.example.test_android_kanji;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Build;
import android.os.Environment;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;


import com.bigfishgames.kanji.KanjiActivity;
import com.bigfishgames.kanji.KanjiGameLib;
import com.example.test_android_kanji.R;

import com.example.test_android_kanji.BuildConfig;
import com.example.test_android_kanji.databinding.ActivityMainBinding;
import com.google.firebase.FirebaseApp;
//import com.example.test_android_kanji.Manifest;

import java.io.File;

public class MainActivity extends KanjiActivity {

    private ActivityMainBinding binding;
    private static final String TAG = "DarkartaMainActivity";
    private boolean gamestarted = false;
    private boolean cameraGranted = false;


    public static boolean syncHints, unlockGame;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        FirebaseApp.initializeApp(this);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        syncHints = false;
        DownloadAssets();
    }

    private void DownloadAssets()
    {

        File obbFile = new File(getObbDir(),"main."+ BuildConfig.VERSION_CODE + ".com.example.test_android_kanji.obb");
        //File obbFile = new File(getObbDir(),"main."+"Test_Version"+ ".in.tuttifrutti.darkarta.abhq.full.obb");
        if (!obbFile.exists())
        {
            final Intent intent = new Intent(MainActivity.this, AssetDownloader.class);
            MainActivity.this.startActivity(intent);
        }
        else
        {
            startGame();
        }
    }

    String gameVersion = "";
    private void startGame()
    {
        Log.d(TAG, "Starting Game");
        gamestarted = true;

        final String deviceName = Build.MODEL;
        int LanguageID = 0;
        final String PublicDCIMFolder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).toString() ;
        boolean skipIntro = false;

        if (BuildConfig.DEBUG)
            skipIntro = true;
        else
            skipIntro = false;

        KanjiGameLib.setConfigVals(BuildConfig.VERSION_CODE, BuildConfig.VERSION_NAME,BuildConfig.CURRENT_MAIN_OBB_VERSION_IN_STORE, deviceName, BuildConfig.DEBUG, BuildConfig.GAME_DEBUG_RES,skipIntro,BuildConfig.GAME_SURVEY_BUILD,LanguageID, this.getResources().getInteger(R.integer.vid_res), gameVersion, this.getResources().getString(R.string.survey_url), this.getResources().getString(R.string.review_url),cameraGranted, PublicDCIMFolder);

    }

    /*
     * A native method that is implemented by the 'test_android_kanji' native library,
     * which is packaged with this application.
     */
    //public native String stringFromJNI();
}