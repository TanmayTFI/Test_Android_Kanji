package com.bigfishgames.kanji;

import java.io.File;
import java.io.FileInputStream;
import android.app.Activity;
import android.media.MediaPlayer;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.util.Log;

public class KanjiMediaPlayer {
	public KanjiMediaPlayer (final Context ctx, final String path) {
      mActivity = (Activity) ctx;
      
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            mMediaCompleted = false;
            mMediaReleased = false;
            mMediaPlayer = new MediaPlayer ();
            mMediaPlayer.setOnCompletionListener (new MediaPlayer.OnCompletionListener() {
               public void onCompletion (MediaPlayer mp) {
                  mMediaCompleted = true;
               }
            });

            try {
               if (path.contains ("/")) {
                  File musicFile = new File (path);
                  FileInputStream is = new FileInputStream (musicFile);
                  
                  mMediaPlayer.setDataSource (is.getFD());
               }
               else {
                  AssetFileDescriptor afd;

                  afd = ctx.getAssets().openFd (path);
                  mMediaPlayer.setDataSource (afd.getFileDescriptor (), afd.getStartOffset (), afd.getLength ());
               }
               
               mMediaPlayer.prepare ();
            } catch (Exception e) {
               Log.v ("Kanji", String.format ("KanjiMediaPlayer: error loading %1$s: %2$s", path, e.getMessage ()));
            }
         }
      });
	}

	public void finalize () {
      if (!mMediaReleased) {
         mMediaReleased = true;
         
         mActivity.runOnUiThread (new Runnable() {
            public void run() {
               try {
                  mMediaPlayer.release ();
               } catch (Exception e) {
               }
            }
         });
      }
	}

	public String play (boolean bLoops) {
      final boolean bFinalLoops = bLoops;
      
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            mMediaCompleted = false;
            try {
               mMediaPlayer.setLooping (bFinalLoops);
               mMediaPlayer.start ();
            } catch (Exception e) {
            }
         }
      });
      
		return null;
	}

	public String setVolume (float fLeftVolume, float fRightVolume) {
      final float fFinalLeftVolume = fLeftVolume, fFinalRightVolume = fRightVolume;
      
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            try {
               mMediaPlayer.setVolume (fFinalLeftVolume, fFinalRightVolume);
            } catch (Exception e) {
            }
         }
      });
      
		return null;
	}

	public String stop () {
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            try {
               mMediaPlayer.stop ();
            } catch (Exception e) {
            }
            mMediaCompleted = true;
         }
      });
      
		return null;
	}

	public String pause () {
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            try {
               mMediaPlayer.pause ();
            } catch (Exception e) {
            }
         }
      });
      
		return null;
	}

   public Integer isPlaying () {
      return mMediaCompleted ? 0 : 1;
   }
   
	public String release () {
      if (!mMediaReleased) {
         mMediaReleased = true;
         
         mActivity.runOnUiThread (new Runnable() {
            public void run() {
               try {
                  mMediaPlayer.release ();
               } catch (Exception e) {
               }
            }
         });
      }
      
		return null;
	}

   private Activity mActivity;
	private MediaPlayer mMediaPlayer;
   private volatile boolean mMediaCompleted = false;
   private volatile boolean mMediaReleased = false;
}

