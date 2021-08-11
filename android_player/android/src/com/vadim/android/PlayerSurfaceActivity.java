package com.vadim.android;

import android.app.Activity;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class PlayerSurfaceActivity {
    private static final String TAG = PlayerSurfaceActivity.class.getName();

    private FrameLayout playerLayout;

    public void onCreate(Activity activity) {
        try {
            final View view = activity.getWindow().getDecorView().getRootView();
            final LinearLayout mainLinearLayout = (LinearLayout)(((ViewGroup)view).getChildAt(0));
            final FrameLayout mainFrameLayout = (FrameLayout)(mainLinearLayout.getChildAt(1));
            final ViewGroup layout = (ViewGroup) mainFrameLayout.getChildAt(0);
            playerLayout = new FrameLayout(activity.getApplicationContext());
            layout.addView(playerLayout, 0, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.MATCH_PARENT));

            layout.setOnHierarchyChangeListener(new ViewGroup.OnHierarchyChangeListener() {
                @Override
                public void onChildViewAdded(View parent, View child) {
                    Log.d(TAG, "onChildViewAdded() called with: parent = [" + parent + "], child = [" + child + "]");
                    if ("org.qtproject.qt5.android.QtSurface".equals(child.getClass().getName())) {
                        layout.removeView(playerLayout);
                        layout.addView(playerLayout, 0);
                    }
                }

                @Override
                public void onChildViewRemoved(View parent, View child) {
                    Log.d(TAG, "onChildViewRemoved() called with: parent = [" + parent + "], child = [" + child + "]");
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void setPlayerSurfaceGeometry(View surfaceView, int x, int y, int width, int height) {
        Log.d(TAG, "setPlayerSurfaceGeometry() surfaceView: " + surfaceView + " x: "
                + x + " y: " + y + " width: " + width + " height: " + height);
        FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(width, height);
        lp.leftMargin = x;
        lp.topMargin = y;
        surfaceView.setLayoutParams(lp);;
    }

    public void addPlayerSurface(View surfaceView, int x, int y, int width, int height) {
        Log.d(TAG, "addPlayerSurface() surfaceView: " + surfaceView + " x: " + x
                + " y: " + y + " width: " + width + " height: " + height);
        FrameLayout.LayoutParams lp;

        if (width > 0 && height > 0) {
            lp = new FrameLayout.LayoutParams(width, height);
            lp.leftMargin = x;
            lp.topMargin = y;
        } else {
            lp = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.MATCH_PARENT);
        }
        if (playerLayout != null) {
            playerLayout.addView(surfaceView, playerLayout.getChildCount(), lp);
        }
    }
}
