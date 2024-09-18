package com.bigfishgames.kanji;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import androidx.core.app.NotificationCompat;
import android.util.Log;

public class KanjiNotificationReceiver extends BroadcastReceiver {
   @Override
   public void onReceive (Context context, Intent intent) {
    	String strMessage = intent.getStringExtra ("k_message");
    	
      try {
        	int nUserData = intent.getIntExtra ("k_userdata", 0);
         NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
         Intent mainIntent = new Intent ();
         String strPackageName = intent.getStringExtra("k_package");
         String strActivityName = intent.getStringExtra("k_activity");
         
         if (!strActivityName.contains("."))
            strActivityName = strPackageName + "." + strActivityName;
         else if (strActivityName.startsWith ("."))
            strActivityName = strPackageName + strActivityName;
         
         Log.i ("Kanji", "KanjiNotificationReceiver: received local notification");
         
         mainIntent.setClassName (strPackageName, strActivityName);
         mainIntent.putExtra ("k_userdata", nUserData);
         mainIntent.putExtra ("k_is_notification", 1);
         
         Notification notif = new NotificationCompat.Builder (context, "") // Builder(Context) is deprecated post Oreo, so chaning it to Builder(Context context, String channelId)
            .setAutoCancel(false)
            .setContentIntent(PendingIntent.getActivity(context, 0,
                                                        mainIntent,
                                                        PendingIntent.FLAG_UPDATE_CURRENT))
            .setContentTitle(intent.getStringExtra("k_title"))
            .setContentText(strMessage)
            .setStyle(new NotificationCompat.BigTextStyle().bigText(strMessage))
            .setDefaults(Notification.DEFAULT_ALL)
            .setSmallIcon(intent.getIntExtra("k_iconresource", 0))
            .setLargeIcon(BitmapFactory.decodeResource(context.getResources(), intent.getIntExtra("k_iconresource", 0)))
            .setTicker(intent.getStringExtra("k_title"))
            .setWhen(System.currentTimeMillis())
            .build();
         
         notificationManager.notify (0, notif);
      } catch (Exception e) {
        	Log.d ("Kanji", "Exception in KanjiNotificationReceiver.onReceive(): " + e.toString());
      }
   }
}
