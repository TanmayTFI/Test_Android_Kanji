package org.nativegame;

import android.view.SurfaceView;

public class NativeFrameClient {
    // Method called when the surface changes
    static private OnSurfaceChangedCallback mOnSurfaceChangedCallback;

    // Method called when a frame starts
	 static private OnFrameStartCallback mOnFrameStartCallback;

    // Method called when a frame ends
	 static private OnFrameEndCallback mOnFrameEndCallback;

    public interface OnSurfaceChangedCallback {
 		 public void onSurfaceChanged(SurfaceView surfaceView, int width, int height);
    }

    public interface OnFrameStartCallback {
 		 public void onFrameStart(int width, int height);
	 }

	 public interface OnFrameEndCallback {
		 public void onFrameEnd();
	 }
	
    // Set callbacks

	 static public void requestNextFrameNotifications(OnSurfaceChangedCallback onSurfaceChangedCallback, OnFrameStartCallback onFrameStartCallback, OnFrameEndCallback onFrameEndCallback) {
       mOnSurfaceChangedCallback = onSurfaceChangedCallback;
		 mOnFrameStartCallback = onFrameStartCallback;
		 mOnFrameEndCallback = onFrameEndCallback;
    }

    // Get surface changed callback

    static public OnSurfaceChangedCallback getOnSurfaceChangedCallback() {
       return mOnSurfaceChangedCallback;
    }

    // Get frame start callback

    static public OnFrameStartCallback getOnFrameStartCallback() {
       return mOnFrameStartCallback;
    }

    // Get frame end callback

    static public OnFrameEndCallback getOnFrameEndCallback() {
       return mOnFrameEndCallback;
    }
};
