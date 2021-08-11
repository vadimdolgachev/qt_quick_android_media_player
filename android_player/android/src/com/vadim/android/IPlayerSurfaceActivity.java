package com.vadim.android;

import android.view.SurfaceView;
import android.view.View;

public interface IPlayerSurfaceActivity {
    void setPlayerSurfaceGeometry(View surfaceView, int x, int y, int width, int height);
    void addPlayerSurface(View surfaceView, int x, int y, int width, int height);
}
