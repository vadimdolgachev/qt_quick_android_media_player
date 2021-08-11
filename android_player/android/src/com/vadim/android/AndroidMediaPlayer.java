package com.vadim.android;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.os.Build;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.Surface;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public final class AndroidMediaPlayer implements
        MediaPlayer.OnCompletionListener,
        MediaPlayer.OnBufferingUpdateListener,
        MediaPlayer.OnErrorListener,
        MediaPlayer.OnInfoListener,
        MediaPlayer.OnPreparedListener,
        MediaPlayer.OnSeekCompleteListener,
        MediaPlayer.OnVideoSizeChangedListener {
    private static final String TAG = "AndroidMediaPlayer";

    private final static int FORCE_NONE = 0;
    private final static int FORCE_ANDROID_MEDIAPLAYER = 1;
    private final static int FORCE_RT_MEDIAPLAYER = 2;
    private final static int FORCE_BY_SCORE = 3;

    private MediaPlayer mMediaPlayer;
    private MediaPlayerEventListener mEventListener;

    public AndroidMediaPlayer() {
        Log.d(TAG, "AndroidMediaPlayer() called");
        mMediaPlayer = new MediaPlayer();
        mMediaPlayer.setOnCompletionListener(this);
        mMediaPlayer.setOnBufferingUpdateListener(this);
        mMediaPlayer.setOnErrorListener(this);
        mMediaPlayer.setOnInfoListener(this);
        mMediaPlayer.setOnPreparedListener(this);
        mMediaPlayer.setOnSeekCompleteListener(this);
        mMediaPlayer.setOnVideoSizeChangedListener(this);
    }

    public void setEventListener(MediaPlayerEventListener eventListener) {
        Log.d(TAG, "setEventListener() called with: eventListener = [" + eventListener + "]");
        mEventListener = eventListener;
    }

    public static AssetFileDescriptor getAssetFileDescriptor(@NonNull Context context, final String source) {
        AssetFileDescriptor assetFd = null;
        try {
            assetFd = context.getAssets().openFd(source);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return assetFd;
    }

    public void setDataSource(final String source) throws Exception {
        Log.d(TAG, "setDataSource() source: " + source);
        try {
            mMediaPlayer.setDataSource(source);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    public void setDataSource(@NonNull AssetFileDescriptor assetFileDescriptor, long offset, long length) throws Exception {
        Log.d(TAG, "setDataSource() playing from assets with fd: " + assetFileDescriptor);
        try {
            mMediaPlayer.setDataSource(assetFileDescriptor.getFileDescriptor(),
                    assetFileDescriptor.getStartOffset(),
                    assetFileDescriptor.getLength());
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    public void prepare() {
        Log.d(TAG, "prepare()");
        mMediaPlayer.prepareAsync();
        if (mEventListener != null) {
            mEventListener.onBuffering(true);
        }
    }

    public void stop() {
        Log.d(TAG, "stop()");
        mMediaPlayer.stop();
    }

    public void reset() {
        Log.d(TAG, "reset()");
        mMediaPlayer.reset();
    }

    public void release() {
        Log.d(TAG, "release()");
        mMediaPlayer.release();
    }

    public void pause() {
        Log.d(TAG, "pause()");
        mMediaPlayer.pause();
        if (mEventListener != null) {
            mEventListener.onPause();
        }
    }

    public void resume() {
        Log.d(TAG, "resume()");
        start();
    }

    public void seekTo(final long mills) {
        Log.d(TAG, "seekTo(): " + mills);
        mMediaPlayer.seekTo((int) mills);
    }

    public long getCurrentPosition() {
        Log.d(TAG, "getCurrentPosition()");
        return mMediaPlayer.getCurrentPosition();
    }

    public long getDuration() {
        Log.d(TAG, "getDuration()");
        return mMediaPlayer.getDuration();
    }

    @Override
    public void onCompletion(MediaPlayer mp) {
        Log.d(TAG, "onCompletion() called with: mp = [" + mp + "]");
        if (mEventListener != null) {
            mEventListener.onFinished();
        }
    }

    @Override
    public void onBufferingUpdate(MediaPlayer mp, int percent) {
        Log.d(TAG, "onBufferingUpdate() called with: mp = [" + mp + "], percent = [" + percent + "]");
    }

    @Override
    public boolean onError(MediaPlayer mp, int what, int extra) {
        Log.d(TAG, "onError() called with: mp = [" + mp + "], what = [" + what + "], extra = [" + extra + "]");
        if (mEventListener != null) {
            mEventListener.onError(what, extra);
        }
        // surface is destroyd. finish playback.
        if (what == 1 && extra == -19) {
            stop();
            reset();
        }
        return false;
    }

    @Override
    public boolean onInfo(MediaPlayer mp, int what, int extra) {
        Log.d(TAG, "onInfo() called with: mp = [" + mp + "], what = [" + what + "], extra = [" + extra + "]");

        switch (what) {
            case MediaPlayer.MEDIA_INFO_VIDEO_RENDERING_START:
                if (mEventListener != null) {
                    mEventListener.onStarted();
                }
                break;
            case MediaPlayer.MEDIA_INFO_BUFFERING_START:
                if (mEventListener != null) {
                    mEventListener.onBuffering(true);
                }
                break;
            case MediaPlayer.MEDIA_INFO_BUFFERING_END:
                if (mEventListener != null) {
                    mEventListener.onBuffering(false);
                }
                break;
        }

        return false;
    }

    @Override
    public void onPrepared(MediaPlayer mp) {
        Log.d(TAG, "onPrepared() called with: mp = [" + mp + "]");
        if (mEventListener != null) {
            mEventListener.onPrepared();
        }
    }

    @Override
    public void onSeekComplete(MediaPlayer mp) {
        Log.d(TAG, "onSeekComplete() called with: mp = [" + mp + "]");
    }

    public void setVideoScalingMode(int mode) {
        Log.d(TAG, "setVideoScalingMode() called with: mode = [" + mode + "]");
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
            mMediaPlayer.setVideoScalingMode(mode);
        }
    }

    public void start() {
        Log.d(TAG, "start()");
        mMediaPlayer.start();
    }

    public void useRTPlayer(boolean flag) {
        Log.d(TAG, "useRTPlayer() called with: useRTPlayer = [" + flag + "]");
        try {
            final Method method = mMediaPlayer.getClass().getMethod("useRTMediaPlayer", int.class);
            method.invoke(mMediaPlayer, flag ? FORCE_RT_MEDIAPLAYER : FORCE_ANDROID_MEDIAPLAYER);
            Log.d(TAG, "useRTPlayer() set success: " + (flag ? FORCE_RT_MEDIAPLAYER : FORCE_ANDROID_MEDIAPLAYER));
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
//            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
        Log.d(TAG, "onVideoSizeChanged() called with: mp = [" + mp + "], width = [" + width + "], height = [" + height + "]");
        if (width == 0 || height == 0) {
            Log.e(TAG, "invalid video width(" + width + ") or height(" + height + ")");
            return;
        }
        if (mEventListener != null) {
            mEventListener.onVideoSizeChanged(width, height);
        }
    }

    public void setSurface(Surface surface) {
        Log.d(TAG, "setSurface() called with: surface = [" + surface + "]");
        try {
            if (surface != null && surface.isValid()) {
                mMediaPlayer.setSurface(surface);
            } else {
                mMediaPlayer.setSurface(null);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
