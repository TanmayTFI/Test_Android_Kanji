package com.bigfishgames.kanji;

import android.media.AudioFormat;
import android.media.AudioAttributes;
import android.media.AudioManager; 
import android.media.AudioTrack; 
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

public class KanjiSound {
   static public void setGameView (KanjiGLSurfaceView view) {
      m_GameView = view;
   }
   
   static public void onPause () {
      if (m_UpdateThread != null && m_UpdateHandler != null) {
         m_UpdateHandler.getLooper().quit ();
         m_UpdateThread = null;
         m_UpdateHandler = null;
      }
   }
   
   static public void onResume () {
      m_UpdateThread = new Thread() {
         public void run () {
            try {
               Looper.prepare ();
               m_UpdateHandler = new Handler();
               Looper.loop ();
            } catch (Exception e) {
            }
         }
      };
      m_UpdateThread.setPriority (Thread.MAX_PRIORITY);
      m_UpdateThread.start ();
   }
      
   static public int getMinSoundBufferSize (int nSampleRate, int nChannels, int nBitsPerSample) {
      return AudioTrack.getMinBufferSize (nSampleRate,
                                          (nChannels == 2) ? AudioFormat.CHANNEL_OUT_STEREO : AudioFormat.CHANNEL_OUT_MONO,
                                          (nBitsPerSample == 16) ? AudioFormat.ENCODING_PCM_16BIT : AudioFormat.ENCODING_PCM_8BIT);
   }
   
	public KanjiSound(int nSampleRate, int nChannels, int nBitsPerSample, int nBufferSize, boolean bLoops, boolean bStatic) {
		mLoops = bLoops;
      mStreaming = false;
		mFrameCount = nBufferSize / nChannels;
		if (nBitsPerSample == 16)
			mFrameCount /= 2;

	  try {

		/*
		// Below constructor is depereated in API 26, so replacing it with below new set of constructors.
		 mAudioTrack = new AudioTrack (AudioManager.STREAM_MUSIC,
										nSampleRate,
										(nChannels == 2) ? AudioFormat.CHANNEL_OUT_STEREO : AudioFormat.CHANNEL_OUT_MONO,
										(nBitsPerSample == 16) ? AudioFormat.ENCODING_PCM_16BIT : AudioFormat.ENCODING_PCM_8BIT,
										nBufferSize,
										(bStatic == false) ? AudioTrack.MODE_STREAM : AudioTrack.MODE_STATIC);
		*/

		  if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
			  mAudioTrack = new  AudioTrack(
					 new AudioAttributes.Builder()
							  .setUsage(AudioAttributes.USAGE_MEDIA)
							  .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
							  .build(),
					  new AudioFormat.Builder()
							  .setSampleRate(nSampleRate)
							  .setEncoding((nBitsPerSample == 16) ? AudioFormat.ENCODING_PCM_16BIT : AudioFormat.ENCODING_PCM_8BIT)
							  .setChannelMask((nChannels == 2) ? AudioFormat.CHANNEL_OUT_STEREO : AudioFormat.CHANNEL_OUT_MONO).build(),
					  nBufferSize,
					  (bStatic == false) ? AudioTrack.MODE_STREAM : AudioTrack.MODE_STATIC,
					  AudioManager.AUDIO_SESSION_ID_GENERATE);
		  } else {
			  //support for Android KitKat
			  mAudioTrack = new AudioTrack (AudioManager.STREAM_MUSIC,
					  nSampleRate,
					  (nChannels == 2) ? AudioFormat.CHANNEL_OUT_STEREO : AudioFormat.CHANNEL_OUT_MONO,
					  (nBitsPerSample == 16) ? AudioFormat.ENCODING_PCM_16BIT : AudioFormat.ENCODING_PCM_8BIT,
					  nBufferSize,
					  (bStatic == false) ? AudioTrack.MODE_STREAM : AudioTrack.MODE_STATIC);
		  }




	  } catch (Exception e) {
	      Log.e("Kanji", "exception while creating AudioTrack: " + e.toString(), e);
	  }
	}
   
   public Integer isReady () {
      if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED)
				   return 1;
				else
				   return 0;
			 }
			 catch (Exception e) {
				return 0;
			 }
		  }
	  }
	  else {
		  return 0;
	  }
   }

	public void finalize () {
	  if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED)
				   mAudioTrack.stop ();
			 }
			 catch (Exception e) {
			 }
         
			 try {
				mAudioTrack.release ();
			 }
			 catch (Exception e) {
			 }
		  }
	  }
	}

	public void writeSamples (byte[] audioData, int nSize) {
	  if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				   mAudioTrack.write (audioData, 0, nSize);
				}
			 }
			 catch (Exception e) {
			 }
		  }
	  }
	}

	public void setVolume (float fLeftVolume, float fRightVolume) {
	  if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				   mAudioTrack.setStereoVolume (fLeftVolume, fRightVolume);
				}
			 }
			 catch (Exception e) {
			 }
		  }
	  }
	}
   
   public void setRate (int nSampleRate) {
      if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				   mAudioTrack.setPlaybackRate (nSampleRate);
				}
			 }
			 catch (Exception e) {
			 }
		  }
	  }
   }

	public void play () {
	  if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				   mAudioTrack.stop ();
				   if (mLoops)
					  mAudioTrack.setLoopPoints (0, mFrameCount, -1);
				   else
					  mAudioTrack.setLoopPoints (0, mFrameCount, 0);
				   mAudioTrack.setPlaybackHeadPosition (0);
               
				   if (!mLoops && !mStreaming) {
					  // Some devices such as the Kindle Fire HDX don't always correctly report the sample position, making isPlaying() report true even though the sample is completed
					  // Register to be called when the sample is past the end marker so we can stop it ourselves as well.
                  
					  mAudioTrack.setNotificationMarkerPosition (mFrameCount);
					  mAudioTrack.setPlaybackPositionUpdateListener (new AudioTrack.OnPlaybackPositionUpdateListener () {
						 public void onMarkerReached (AudioTrack track) {
							try {
							   mAudioTrack.stop ();
							}
							catch (Exception e) {
							}
						 }
                     
						 public void onPeriodicNotification (AudioTrack track) {
						 }
					  }, m_UpdateHandler);
				   }
               
				   mAudioTrack.play ();
				}
			 }
			 catch (Exception e) {
			 }
		  }
	  }
	}

	public void stop () {
	  if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				   mAudioTrack.stop ();
				   mAudioTrack.setPlaybackPositionUpdateListener (null);
				}
			 }
			 catch (Exception e) {
			 }
		  }
	  }
	}

	public void pause () {
	  if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				   mAudioTrack.pause ();
				}
			 }
			 catch (Exception e) {
			 }
		  }
	  }
	}

	public void release () {
	  if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				mAudioTrack.setPlaybackPositionUpdateListener (null);
				mAudioTrack.release ();
			 }
			 catch (Exception e) {
			 }
		  }
	  }
	}

	public Integer isPlaying () {
	  if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				if (mAudioTrack.getState() != AudioTrack.STATE_UNINITIALIZED) {
				   return (mAudioTrack.getPlayState() == AudioTrack.PLAYSTATE_PLAYING && mAudioTrack.getPlaybackHeadPosition() < mFrameCount) ? 1 : 0;
				}
				else
				   return 0;
			 }
			 catch (Exception e) {
				return 0;
			 }
		  }
	  }
	  else {
	      return 0;
	  }
	}
   
   public void flushSamples () {
      if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				mAudioTrack.flush ();
			 }
			 catch (Exception e) {
			 }
		  }
	  }
   }
   
   public void setPeriodicNotification (int nPeriod, int nUserData) {
      if (mAudioTrack != null) {
		  synchronized (mAudioTrack) {
			 try {
				mNotificationUserData = nUserData;
				mStreaming = true;
            
				mAudioTrack.setPlaybackPositionUpdateListener (new AudioTrack.OnPlaybackPositionUpdateListener () {
				   public void onMarkerReached (AudioTrack track) {
				   }
               
				   public void onPeriodicNotification (AudioTrack track) {
					  if (m_GameView != null) {
						 try {
							m_GameView.onPeriodicAudioNotification (mNotificationUserData);
						 } catch (Exception e) {
						 }
					  }
				   }
				}, m_UpdateHandler);
            
				mAudioTrack.setPositionNotificationPeriod (nPeriod);
			 }
			 catch (Exception e) {
			 }
		  }
	  }
   }

	private AudioTrack mAudioTrack;
	private int mFrameCount;
	private boolean mLoops;
   private boolean mStreaming;
   private int mNotificationUserData = 0;
   static private KanjiGLSurfaceView m_GameView = null;
   static private Thread m_UpdateThread = null;
   static private Handler m_UpdateHandler = null;
}
