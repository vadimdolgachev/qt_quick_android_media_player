package com.vadim.android;

import android.content.Context;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.FrameLayout;

public final class PlayerSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    private static final String TAG = PlayerSurfaceView.class.getName();

    public static final int SCALING_TO_FILL_MODE = 0;
    public static final int SCALING_TO_FIT_MODE = 1;
    public static final int SCALING_TO_PAN_AND_SCAN_MODE = 2;

    private SurfaceChangeListener surfaceChangeListener;
    private int mVideoWidth;
    private int mVideoHeight;
    private int mScalingMode = SCALING_TO_FIT_MODE;

    public PlayerSurfaceView(Context context) {
        super(context);
        getHolder().addCallback(this);
    }

    public void setSurfaceChangeListener(SurfaceChangeListener surfaceChangeListener) {
        this.surfaceChangeListener = surfaceChangeListener;
    }

    public PlayerSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        getHolder().addCallback(this);
    }

    public PlayerSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        getHolder().addCallback(this);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public PlayerSurfaceView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        getHolder().addCallback(this);
    }

    public void setVideoSize(int playerWidth, int playerHeight) {
        mVideoWidth = playerWidth;
        mVideoHeight = playerHeight;
        getHolder().setFixedSize(playerWidth, playerHeight);
        requestLayout();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        Log.d(TAG, "onMeasure() called with: widthMeasureSpec = [" + widthMeasureSpec + "], heightMeasureSpec = [" + heightMeasureSpec + "]");
        int width = getDefaultSize(mVideoWidth, widthMeasureSpec);
        int height = getDefaultSize(mVideoHeight, heightMeasureSpec);
        Log.d(TAG, "onMeasure() width = [" + width + "], height = [" + height + "]");

        float aspectWidth = mVideoWidth;
        float aspectHeight = mVideoHeight;

        int scaleType = mScalingMode;

        View parent = (View)getParent();
        width = parent.getWidth();
        height = parent.getHeight();

        if (scaleType == SCALING_TO_FIT_MODE) {
            if (aspectWidth * height < width * aspectHeight) {
                // reduce width
                width = Math.round(height * aspectWidth / aspectHeight);
            } else if (aspectWidth * height  > width * aspectHeight) {
                // reduce height
                height = Math.round(width * aspectHeight / aspectWidth);
            }
        } else if (scaleType == SCALING_TO_PAN_AND_SCAN_MODE) {
            if (aspectWidth * height > width * aspectHeight) {
                // increase width
                width = Math.round(height * aspectWidth / aspectHeight);
                Log.d(TAG, "onMeasure: increase width: " + width);
            } else if (aspectWidth * height  < width * aspectHeight) {
                // increase height
                height = Math.round(width * aspectHeight / aspectWidth);
                Log.d(TAG, "onMeasure: increase height: " + height);
            }
        } else if (scaleType == SCALING_TO_FILL_MODE) {
            //
        }

        Log.d(TAG, "onMeasure() width: " + width + " height: " + height);
        setMeasuredDimension(width, height);
        if (parent.getWidth() > 0 && parent.getHeight() > 0 && mVideoWidth > 0 && mVideoHeight > 0) {
            FrameLayout.LayoutParams layoutParams = (FrameLayout.LayoutParams) getLayoutParams();
            layoutParams.leftMargin = (int) ((parent.getWidth() - width)/2.f);
            layoutParams.rightMargin = (int) ((parent.getWidth() - width)/2.f);
            layoutParams.topMargin = (int) ((parent.getHeight() - height)/2.f);
            layoutParams.bottomMargin = (int) ((parent.getHeight() - height)/2.f);
            setLayoutParams(layoutParams);
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.d(TAG, "surfaceCreated() called with: holder = [" + holder + "]");
        onSurfaceChanged(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.d(TAG, "surfaceChanged() called with: holder = [" + holder + "], format = [" + format + "], width = [" + width + "], height = [" + height + "]");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.d(TAG, "surfaceDestroyed() called with: holder = [" + holder + "]");
        onSurfaceChanged(null);
    }

    private void onSurfaceChanged(Surface surface) {
        Log.d(TAG, "onSurfaceChanged() called with: surface = [" + surface + "]");
        if (surfaceChangeListener != null) {
            surfaceChangeListener.onSurfaceChange(surface);
        }
    }

    public int getScalingMode() {
        return mScalingMode;
    }

    public void setScalingMode(int scalingMode) {
        if (scalingMode != mScalingMode) {
            mScalingMode = scalingMode;
            requestLayout();
        }
    }
}
