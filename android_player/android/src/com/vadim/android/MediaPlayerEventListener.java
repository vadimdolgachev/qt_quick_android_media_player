package com.vadim.android;

interface MediaPlayerEventListener {
    void onVideoSizeChanged(int width, int height);
    void onStarted();
    void onFinished();
    void onError(int what, int extra);
    void onBuffering(boolean state);
    void onPause();
    void onPrepared();
}
