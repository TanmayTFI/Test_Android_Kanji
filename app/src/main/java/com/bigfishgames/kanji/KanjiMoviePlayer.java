package com.bigfishgames.kanji;

import java.io.File;
import java.io.FileInputStream;
import android.media.MediaPlayer;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.os.Build;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.SurfaceView;
import android.view.SurfaceHolder;

public class KanjiMoviePlayer {
	public KanjiMoviePlayer (final Context ctx, final String path, 
                            final int x1, final int y1, final int x2, final int y2,
                            final boolean bVideoLoops, final float fLeftVolume, final float fRightVolume, final boolean bPlayBehindKWindow) {
      mActivity = (KanjiActivity) ctx;
      mPlayBehindKWindow = bPlayBehindKWindow;
      mMovieLoops = bVideoLoops;
      mMoviePaused = false;
      
      Log.v ("Kanji", "KanjiMoviePlayer: play " + path);
      
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            mMovieCompleted = false;
            mMoviePlayer = new MediaPlayer ();
            
            if (android.os.Build.VERSION.SDK_INT >= 17) {
               Log.v ("Kanji", "KanjiMoviePlayer: use API level 17 features");
               mMoviePlayer.setOnInfoListener (new MediaPlayer.OnInfoListener() {
                  public boolean onInfo (MediaPlayer mp, int what, int extra) {
                     if (what == MediaPlayer.MEDIA_INFO_VIDEO_RENDERING_START) {
                        mMoviePrepared = true;
                        Log.v ("Kanji", "KanjiMoviePlayer: rendering starts");
                     }
                     
                     return false;
                  }
               });
            }
            else {
               mMoviePlayer.setOnPreparedListener (new MediaPlayer.OnPreparedListener() {
                  public void onPrepared (MediaPlayer mp) {
                     mMoviePrepared = true;
                  }
               });
            }
            
            mMoviePlayer.setOnErrorListener (new MediaPlayer.OnErrorListener() {
               public boolean onError (MediaPlayer mp, int what, int extra) {
                  if (what == MediaPlayer.MEDIA_ERROR_UNKNOWN || what == -38) {
                     /* Video error'ed out when resumed after the activity itself was resumed - occurs on some
                      * devices. Seek to the position at suspend time and mark the error as processed. */
                     Log.v ("Kanji", "KanjiMoviePlayer: recover from error " + what + ", " + extra + "; seek to position " + mPausedVideoPosition);
                     if (mPausedVideoPosition >= 0 && mp.isPlaying() && !mMoviePaused) {
                        mp.seekTo (mPausedVideoPosition);
                     }
                     return true;
                  }
                  else {
                     /* Fall through to the onCompletion listener */
                     Log.v ("Kanji", "KanjiMoviePlayer: caught error " + what + ", " + extra);
                     return false;
                  }
               }
            });
            
            mMoviePlayer.setOnCompletionListener (new MediaPlayer.OnCompletionListener() {
               public void onCompletion (MediaPlayer mp) {
                  if (!mMovieLoops) {
                     Log.v ("Kanji", "KanjiMoviePlayer: movie completed");
                     mMovieCompleted = true;
                     if (mActivity != null && mActivity.view != null) {
                        mActivity.view.onUserEvent (116);   /* K_EVENT_VIDEO_ENDED */
                     }
                  }
               }
            });
            
            m_x1 = x1;
            m_y1 = y1;
            m_x2 = x2;
            m_y2 = y2;
            
            mIgnoreNextSurfaceChange = 1;
            mMovieView = new SurfaceView (mActivity);
            
            SurfaceHolder holder = mMovieView.getHolder ();
            holder.setFixedSize (m_x2 - m_x1, m_y2 - m_y1);
            if(android.os.Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN) // SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS deprecated after API 16
               holder.setType (SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
            holder.addCallback (new SurfaceHolder.Callback () {
               public void surfaceCreated (SurfaceHolder holder) {
                  mMovieView.layout (m_x1, m_y1, m_x2, m_y2);
                  mMoviePlayer.setDisplay (holder);
                  
                  try {
                     if (path.contains ("/")) {
                        File musicFile = new File (path);
                        FileInputStream is = new FileInputStream (musicFile);
                        
                        mMoviePlayer.setDataSource (is.getFD());
                     }
                     else {
                        AssetFileDescriptor afd;
                        
                        afd = ctx.getAssets().openFd (path);
                        mMoviePlayer.setDataSource (afd.getFileDescriptor (), afd.getStartOffset (), afd.getLength ());
                     }
                     
                     mMoviePlayer.prepare ();
                     
                     mMoviePlayer.setLooping (bVideoLoops);
                     mMoviePlayer.setVolume (fLeftVolume, fRightVolume);
                     mMoviePlayer.start ();
                  } catch (Exception e) {
                     Log.v ("Kanji", String.format ("KanjiMoviePlayer: error loading %1$s: %2$s", path, e.getMessage ()));
                     if (!mMoviePrepared) {
                        Log.v ("Kanji", "KanjiMoviePlayer: movie completed (failed to load)");
                        mMovieCompleted = true;
                     }
                  }
               }
               public void surfaceDestroyed (SurfaceHolder holder) {
               }
               public void surfaceChanged (SurfaceHolder holder, int format, int width, int height) {
                  if (mIgnoreNextSurfaceChange > 0)
                     mIgnoreNextSurfaceChange --;
                  else
                     mMovieSurfaceChanged = true;
                  mMovieView.layout (m_x1, m_y1, m_x2, m_y2);
               }
            });
            
            mActivity.addContentView (mMovieView, new LayoutParams (LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT)); // FILL_PARENT deprecated post API 8, using MATCH_PARENT
            mMovieView.setVisibility (View.VISIBLE);
            if (mPlayBehindKWindow) {
               /* Show behind KWindow */
               mMovieView.setZOrderMediaOverlay (false);
               mActivity.view.setKanjiInFront (true);
            }
            else {
               /* Show in front of KWindow */
               mMovieView.setZOrderMediaOverlay (true);
               mActivity.view.setKanjiInFront (false);
            }
         }
      });
	}

	public void finalize () {
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            try {
               Log.v ("Kanji", "KanjiMoviePlayer: finalize");
               if (mMoviePlayer != null) {
                  if (!mMovieCompleted)
                     mMoviePlayer.stop ();
                  mMoviePlayer.release ();
                  mMoviePlayer = null;
               }
               
               if (mMovieView != null) {
                  ((ViewGroup) mMovieView.getParent()).removeView (mMovieView);
                  mMovieView = null;
               }
               
               if (mPlayBehindKWindow)
                  mActivity.view.setKanjiInFront (false);
               
               mPlayBehindKWindow = false;
               mMovieLoops = false;
               mMoviePaused = false;
            } catch (Exception e) {
               Log.v ("Kanji", "KanjiMoviePlayer: caught exception while finalizing: " + e.toString());
            }
         }
      });
	}
   
   public Integer isVisible () {
      if (mMovieLoops) {
         return (mMoviePrepared) ? 1 : 0;         
      }
      else {
         int nCurPosition = -1;
         
         if (mMoviePrepared) {
            try {
               nCurPosition = mMoviePlayer.getCurrentPosition ();
            } catch (Exception e) {
               nCurPosition = -1;
            }
         }
         
         return (mMoviePrepared && nCurPosition >= 10) ? 1 : 0;
      }
   }
   
   public Integer isPlaying () {
      return mMovieCompleted ? 0 : 1;
   }

	public String suspend () {
      mMoviePaused = true;
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            try {
               mPausedVideoPosition = mMoviePlayer.getCurrentPosition();
               if (mMoviePlayer.isPlaying ()) {
                  Log.v ("Kanji", "KanjiMoviePlayer: pause movie at position " + mPausedVideoPosition);
                  mMoviePlayer.pause ();
               }
               ((ViewGroup) mMovieView.getParent()).removeView (mMovieView);
            } catch (Exception e) {
               Log.v ("Kanji", "KanjiMoviePlayer: caught exception while pausing: " + e.toString());
            }
         }
      });
      
		return null;
	}
   
	public String resume () {
      mMoviePaused = false;
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            try {
               if (!mMovieCompleted) {
                  mActivity.addContentView (mMovieView, new LayoutParams (LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));// FILL_PARENT deprecated post API 8, using MATCH_PARENT
                  mMovieView.setVisibility (View.VISIBLE);
                  if (mPlayBehindKWindow) {
                     /* Show behind KWindow */
                     mMovieView.setZOrderMediaOverlay (false);
                     mActivity.view.setKanjiInFront (true);
                  }
                  else {
                     /* Show in front of KWindow */
                     mMovieView.setZOrderMediaOverlay (true);
                     mActivity.view.setKanjiInFront (false);
                  }
                  
                  Log.v ("Kanji", "KanjiMoviePlayer: resume movie");
                  try {
                     if (mPausedVideoPosition >= 0) {
                        Log.v ("Kanji", "KanjiMoviePlayer: resume: seek to position " + mPausedVideoPosition);
                        mMoviePlayer.seekTo (mPausedVideoPosition);
                     }
                     else {
                        Log.v ("Kanji", "KanjiMoviePlayer: resume: no position stored");
                     
                     }
                     mMoviePlayer.start ();
                  } catch (Exception e) {
                     Log.v ("Kanji", "KanjiMoviePlayer: caught exception while resuming: " + e.toString());
                  }
                  
               }
            } catch (Exception e) {
               Log.v ("Kanji", "KanjiMoviePlayer: caught exception while resuming: " + e.toString());
            }
         }
      });
      
		return null;
	}
   
	public String updateVideo () {
      if (mMovieSurfaceChanged) {
         mMovieSurfaceChanged = false;
         
         if (android.os.Build.MANUFACTURER.equalsIgnoreCase ("amazon") && android.os.Build.MODEL.equalsIgnoreCase ("Kindle Fire")) {
            mActivity.runOnUiThread (new Runnable() {
               public void run() {
                  try {
                     if (!mMovieCompleted && mPlayBehindKWindow) {
                        // Fix video layering that gets messed up by the soft keys on the Kindle Fire 1st gen
                        Log.v ("Kanji", "KanjiMoviePlayer: fix layering");
                        mIgnoreNextSurfaceChange = 2;
                        ((ViewGroup) mMovieView.getParent()).removeView (mMovieView);
                        mMovieView.setZOrderMediaOverlay (true);
                        mActivity.view.setKanjiInFront (false);
                        mActivity.addContentView (mMovieView, new LayoutParams (LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));// FILL_PARENT deprecated post API 8, using MATCH_PARENT
                        mMovieView.setVisibility (View.VISIBLE);
                        mMovieView.setZOrderMediaOverlay (false);
                        mActivity.view.setKanjiInFront (true);
                        mMovieView.layout (m_x1, m_y1, m_x2, m_y2);
                     }
                  } catch (Exception e) {
                     Log.v ("Kanji", "KanjiMoviePlayer: caught exception while fixing video layering: " + e.toString());
                  }
               }
            });
         }
      }
      
		return null;
	}
   
	public String setVolume (float fLeftVolume, float fRightVolume) {
      final float fFinalLeftVolume = fLeftVolume, fFinalRightVolume = fRightVolume;
      
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            mMoviePlayer.setVolume (fFinalLeftVolume, fFinalRightVolume);
         }
      });
      
		return null;
	}
   
	public String release () {
      mMoviePaused = false;
      mActivity.runOnUiThread (new Runnable() {
         public void run() {
            try {
               Log.v ("Kanji", "KanjiMoviePlayer: release");
               if (mMoviePlayer != null) {
                  if (!mMovieCompleted)
                     mMoviePlayer.stop ();
                  mMoviePlayer.release ();
                  mMoviePlayer = null;
               }
               
               if (mMovieView != null) {
                  ((ViewGroup) mMovieView.getParent()).removeView (mMovieView);
                  mMovieView = null;
               }
               
               if (mPlayBehindKWindow)
                  mActivity.view.setKanjiInFront (false);
               
               mPlayBehindKWindow = false;
               mMovieLoops = false;
               mMoviePaused = false;
            } catch (Exception e) {
               Log.v ("Kanji", "KanjiMoviePlayer: caught exception while releasing: " + e.toString());
            }
         }
      });
      
		return null;
	}

   private KanjiActivity mActivity;
	private MediaPlayer mMoviePlayer;
   private SurfaceView mMovieView;
   private int m_x1 = 0, m_y1 = 0, m_x2 = 0, m_y2 = 0;
   private int mPausedVideoPosition = -1;
   private boolean mPlayBehindKWindow = false;
   private boolean mMovieLoops = false;
   private volatile boolean mMoviePrepared = false;
   private volatile boolean mMovieCompleted = false;
   private volatile boolean mMoviePaused = false;
   private volatile boolean mMovieSurfaceChanged = false;
   private volatile long mIgnoreNextSurfaceChange = 0;
}

