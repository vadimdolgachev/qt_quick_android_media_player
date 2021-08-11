package com.vadim.android;

import android.os.Bundle;
import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;

import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.util.Log;


import java.io.IOException;

public class MainActivity extends org.qtproject.qt5.android.bindings.QtActivity implements IPlayerSurfaceActivity {
    private PlayerSurfaceActivity playerSurfaceActivity = new PlayerSurfaceActivity();

    public void requestPermissionForReadExtertalStorage(Context context) throws Exception {
        final int READ_STORAGE_PERMISSION_REQUEST_CODE = 0x3;
        try {
            ActivityCompat.requestPermissions((Activity) context,
                    new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
                    READ_STORAGE_PERMISSION_REQUEST_CODE);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    @Override
    public void onCreate(Bundle b) {
        super.onCreate(b);
        try {
            requestPermissionForReadExtertalStorage(this);
        } catch (Exception e) {
            e.printStackTrace();
        }
        playerSurfaceActivity.onCreate(this);
    }

    @Override
    public void setPlayerSurfaceGeometry(View surfaceView, int x, int y, int width, int height) {
        playerSurfaceActivity.setPlayerSurfaceGeometry(surfaceView, x, y, width, height);
    }

    @Override
    public void addPlayerSurface(View surfaceView, int x, int y, int width, int height) {
        playerSurfaceActivity.addPlayerSurface(surfaceView, x, y, width, height);
    }
}
