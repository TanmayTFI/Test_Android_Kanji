package com.example.test_android_kanji;

import static com.google.android.gms.common.util.CollectionUtils.listOf;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.material.snackbar.Snackbar;
import com.google.firebase.FirebaseApp;
import com.google.firebase.storage.FirebaseStorage;
import com.tonyodev.fetch2.Download;
import com.tonyodev.fetch2.Error;
import com.tonyodev.fetch2.Fetch;
import com.tonyodev.fetch2.Request;
import com.tonyodev.fetch2.Status;
import com.tonyodev.fetch2core.Extras;
import com.tonyodev.fetch2core.FetchObserver;
import com.tonyodev.fetch2core.Func;
import com.tonyodev.fetch2core.MutableExtras;
import com.tonyodev.fetch2core.Reason;

import com.tonyodev.fetch2.FetchConfiguration;
import com.tonyodev.fetch2core.Downloader;
import com.tonyodev.fetch2okhttp.OkHttpDownloader;


import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.text.DecimalFormat;
import java.util.Collections;
import java.util.concurrent.TimeUnit;

import com.example.test_android_kanji.BuildConfig;

import okhttp3.OkHttpClient;
import okhttp3.Protocol;

public class AssetDownloader extends AppCompatActivity implements FetchObserver<Download>
{

    private static final int STORAGE_PERMISSION_CODE = 100;

    private View mainView;
    private TextView progressTextView;
    private TextView titleTextView;
    private TextView etaTextView;
    private TextView downloadSpeedTextView;
    private ProgressBar progress_Bar;
    private Request request;
    private Fetch fetch;
    private FirebaseStorage storage;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.asset_downloader);
        mainView = findViewById(R.id.asset_downloader);
        progressTextView = findViewById(R.id.progressTextView);
        titleTextView = findViewById(R.id.titleTextView);
        etaTextView = findViewById(R.id.etaTextView);
        downloadSpeedTextView = findViewById(R.id.downloadSpeedTextView);
        progress_Bar = findViewById(R.id.progressBar);

        //storage = FirebaseStorage.getInstance();

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        OkHttpClient client = new OkHttpClient.Builder()
                .connectTimeout(30, TimeUnit.SECONDS)
                .readTimeout(30, TimeUnit.SECONDS)
                .writeTimeout(30, TimeUnit.SECONDS)
                .protocols(Collections.singletonList(Protocol.HTTP_1_1))
                .build();


        final FetchConfiguration fetchConfiguration = new FetchConfiguration.Builder(this)
                .enableLogging(true)
                .setDownloadConcurrentLimit(4)
                .setHttpDownloader(new OkHttpDownloader(Downloader.FileDownloaderType.PARALLEL))
                .build();
        fetch = Fetch.Impl.getInstance(fetchConfiguration);
        enqueueDownload();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (request != null) {
            fetch.attachFetchObserversForDownload(request.getId(), this);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (request != null) {
            fetch.removeFetchObserversForDownload(request.getId(), this);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        fetch.close();
    }

    @Override
    public void onChanged(Download data, @NotNull Reason reason)
    {
        Log.d("Update Progress ", reason.toString());
        updateViews(data, reason);
    }

    private String lang;
    private String url = "----";
    private void enqueueDownload()
    {
        url = "http://www.tuttifrutti.in/DarkartaMobileAssets/assets.obb";//"http://www.tuttifrutti.in/DarkartaMobileAssets/assets.obb";//"https://storage.googleapis.com/heinz_test_storage/assets_full.obb"
        final String filePath = this.getObbDir()+ "/" + Uri.parse(url).getLastPathSegment();

        Log.d("DL Location", filePath);
        request = new Request(url, filePath);
        request.setExtras(getExtrasForRequest(request));

        fetch.attachFetchObserversForDownload(request.getId(), this)
                .enqueue(request, new Func<Request>() {
                    @Override
                    public void call(@NotNull Request result) {
                        request = result;
                    }
                }, new Func<Error>() {
                    @Override
                    public void call(@NotNull Error result) {
                        //Timber.d("SingleDownloadActivity Error: %1$s", result.toString());
                    }
                });
    }

    private Extras getExtrasForRequest(Request request) {
        final MutableExtras extras = new MutableExtras();
        extras.putBoolean("testBoolean", true);
        extras.putString("testString", "test");
        extras.putFloat("testFloat", Float.MIN_VALUE);
        extras.putDouble("testDouble",Double.MIN_VALUE);
        extras.putInt("testInt", Integer.MAX_VALUE);
        extras.putLong("testLong", Long.MAX_VALUE);
        return extras;
    }

    private void updateViews(@NotNull Download download, Reason reason)
    {

        if(reason == Reason.DOWNLOAD_COMPLETED)
        {
            File oldName = new File(this.getObbDir()+ "/" + Uri.parse(url).getLastPathSegment());
            File newName = new File(this.getObbDir()+ "/main." + BuildConfig.VERSION_CODE + ".com.example.test_android_kanji.obb");

            if(oldName.renameTo(newName))
            {
                System.out.println("renamed");

                doRestart(AssetDownloader.this);

            } else {
                System.out.println("Error");
            }
        }

        if (request.getId() == download.getId()) {
            if (reason == Reason.DOWNLOAD_QUEUED || reason == Reason.DOWNLOAD_COMPLETED) {
                setTitleView("Downloading Assets...");//download.getFile());
            }
            setProgressView(download.getStatus(), download.getProgress());
            etaTextView.setText(getETAString(this, download.getEtaInMilliSeconds()));
            downloadSpeedTextView.setText(getDownloadSpeedString(this, download.getDownloadedBytesPerSecond()));
            if (download.getError() != Error.NONE) {
                showDownloadErrorSnackBar(download.getError());
            }
        }
    }

    public static void doRestart(Context c) {
        try {
            //check if the context is given
            if (c != null) {
                //fetch the packagemanager so we can get the default launch activity
                // (you can replace this intent with any other activity if you want
                PackageManager pm = c.getPackageManager();
                //check if we got the PackageManager
                if (pm != null) {
                    //create the intent with the default start activity for your application
                    Intent mStartActivity = pm.getLaunchIntentForPackage(
                            c.getPackageName()
                    );
                    if (mStartActivity != null) {
                        mStartActivity.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                        //create a pending intent so the application is restarted after System.exit(0) was called.
                        // We use an AlarmManager to call this intent in 100ms
                        int mPendingIntentId = 223344;
                        PendingIntent mPendingIntent = PendingIntent.getActivity(c, mPendingIntentId, mStartActivity,
                                PendingIntent.FLAG_CANCEL_CURRENT | PendingIntent.FLAG_IMMUTABLE);
                        AlarmManager mgr = (AlarmManager) c.getSystemService(Context.ALARM_SERVICE);
                        mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 100, mPendingIntent);
                        //kill the application
                        System.exit(0);
                    }
                }
            }
        } catch (Exception ex) {
            //Log.e(TAG, "Was not able to restart application");
        }
    }

    private void setTitleView(@NonNull final String fileName) {
        final Uri uri = Uri.parse(fileName);
        titleTextView.setText(uri.getLastPathSegment());
    }

    private void setProgressView(@NonNull final Status status, final int progress) {

        Log.d("Progress view update", "Setting progress view " + status);

        switch (status) {
            case QUEUED: {
                progressTextView.setText("Please wait...");
                break;
            }
            case ADDED: {
                progressTextView.setText("ADDED");
                break;
            }
            case DOWNLOADING:
            case COMPLETED: {
                if (progress == -1) {
                    progressTextView.setText("Downloading...");
                } else {
                    final String progressString = progress + "%";//getResources().getString(R.string.percent_progress, progress);
                    progressTextView.setText(progressString);
                    progress_Bar.setProgress(progress);
                }
                break;
            }
            default: {
                progressTextView.setText("STATUS UNKNOWN");
                if(url == "----")
                {
                    url = "http://www.tuttifrutti.in/DarkartaMobileAssets/assets.obb"; //"http://www.tuttifrutti.in/DarkartaMobileAssets/assets.obb";
                }
                break;
            }
        }
    }

    private void showDownloadErrorSnackBar(@NotNull Error error) {
        final Snackbar snackbar = Snackbar.make(mainView, "Download Failed: ErrorCode: " + error, Snackbar.LENGTH_INDEFINITE);
        snackbar.setAction("RETRY", new View.OnClickListener(){
            @Override
            public void onClick(View view) {

                fetch.retry(request.getId());
                snackbar.dismiss();
            }
        });
        snackbar.show();
    }

    @NonNull
    public static String getETAString(@NonNull final Context context, final long etaInMilliSeconds) {
        if (etaInMilliSeconds < 0) {
            return "";
        }
        int seconds = (int) (etaInMilliSeconds / 1000);
        long hours = seconds / 3600;
        seconds -= hours * 3600;
        long minutes = seconds / 60;
        seconds -= minutes * 60;
        if (hours > 0) {
            return hours + "h " + minutes + "m " + seconds + "s left";
        } else if (minutes > 0) {
            return minutes + "m " + seconds + "s left";
        } else {
            return seconds + "s left";
        }
    }

    @NonNull
    public static String getDownloadSpeedString(@NonNull final Context context, final long downloadedBytesPerSecond) {
        if (downloadedBytesPerSecond < 0) {
            return "";
        }
        double kb = (double) downloadedBytesPerSecond / (double) 1000;
        double mb = kb / (double) 1000;
        final DecimalFormat decimalFormat = new DecimalFormat(".##");
        if (mb >= 1) {
            return decimalFormat.format(mb) + "MB/s";
        } else if (kb >= 1) {
            return decimalFormat.format(kb) + "KB/s";
        } else {
            return downloadedBytesPerSecond + "B/s";
        }
    }
}
