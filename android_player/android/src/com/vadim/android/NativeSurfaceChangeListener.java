package com.vadim.android;

import android.view.Surface;

public final class NativeSurfaceChangeListener implements SurfaceChangeListener {
    private long mNativeListener;

    public NativeSurfaceChangeListener(long nativeListener) {
        mNativeListener = nativeListener;
    }

    @Override
    public void onSurfaceChange(Surface surface) {
        onSurfaceChanged(mNativeListener, surface);
    }

    public static native void onSurfaceChanged(long nativeHandle, Surface surface);
}
