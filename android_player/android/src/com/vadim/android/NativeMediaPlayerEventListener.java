package com.vadim.android;

public final class NativeMediaPlayerEventListener implements MediaPlayerEventListener {
    private static final String TAG = NativeMediaPlayerEventListener.class.getName();

    private long mNativeHandler;

    public NativeMediaPlayerEventListener(long nativeHandler) {
        mNativeHandler = nativeHandler;
    }

    @Override
    public void onVideoSizeChanged(int width, int height) {
        onVideoSizeChanged(mNativeHandler, width, height);
    }

    @Override
    public void onStarted() {
        onStarted(mNativeHandler);
    }

    @Override
    public void onFinished() {
        onFinished(mNativeHandler);
    }

    @Override
    public void onError(int what, int extra) {
        onError(mNativeHandler, what, extra);
    }

    @Override
    public void onBuffering(boolean state) {
        onBuffering(mNativeHandler, state);
    }

    @Override
    public void onPause() {
        onPause(mNativeHandler);
    }

    @Override
    public void onPrepared() {
        onPrepared(mNativeHandler);
    }

    public static native void onFinished(long nativeHandle);

    public static native void onStarted(long nativeHandle);

    public static native void onError(long nativeHandle, int what, int extra);

    public static native void onBuffering(long nativeHandle, boolean state);

    public static native void onPause(long nativeHandle);

    public static native void onPrepared(long nativeHandle);

    public static native void onVideoSizeChanged(long nativeHandle, int playerWidth, int playerHeight);
}
