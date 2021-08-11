package com.vadim.android;

import android.graphics.SurfaceTexture;
import android.util.Log;

public final class SurfaceTextureListener implements SurfaceTexture.OnFrameAvailableListener
{
    private long m_callback = 0;

    public SurfaceTextureListener(long callback)
    {
        m_callback = callback;
    }

    @Override
    public void onFrameAvailable (SurfaceTexture surfaceTexture)
    {
        // call the native method
//        Log.d("STL", "onFrameAvailable " + System.currentTimeMillis());
        frameAvailable(m_callback);
    }

//    public native void frameAvailable();
    public native void frameAvailable(long nativeHandle);
}
