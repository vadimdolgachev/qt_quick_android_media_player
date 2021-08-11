#include "AndroidMediaPlayer.h"
#include "AndroidSurfaceView.h"
#include "QSurfaceTexture.h"
#include "com_vadim_android_NativeMediaPlayerEventListener.h"

#include <QtAndroid>
#include <QAndroidJniEnvironment>

const static auto EVENT_LISTENER = std::make_tuple("setEventListener",
                                                   "(Lcom/vadim/android/MediaPlayerEventListener;)V");
enum MediaError {
    MEDIA_ERROR_UNKNOWN = 1,
    MEDIA_ERROR_SERVER_DIED = 100,
    MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK = 200,
    MEDIA_ERROR_IO = -1004,
    MEDIA_ERROR_MALFORMED = -1007,
    MEDIA_ERROR_UNSUPPORTED = -1010,
    MEDIA_ERROR_TIMED_OUT = -110
};

AndroidMediaPlayer::AndroidMediaPlayer(QObject *parent) :
    QObject(parent),
    mPlaybackState(PlaybackState::Idle),
    mUseRTPlayer(false),
    mAutoStart(false)
{
    initAndroidPlayer();
    //    setUseRTPlayer(mUseRTPlayer);
}

AndroidMediaPlayer::~AndroidMediaPlayer()
{
    mAndroidPlayer.callMethod<void>(std::get<0>(EVENT_LISTENER),
                                    std::get<1>(EVENT_LISTENER),
                                    nullptr);
    if (mPlaybackState != PlaybackState::Idle) {
        stop();
        reset();
    }
    release();
}

QQuickItem *AndroidMediaPlayer::surfaceView() const
{
    return mSurfaceView;
}

AndroidMediaPlayer::PlaybackState AndroidMediaPlayer::playbackState() const
{
    return mPlaybackState;
}

void AndroidMediaPlayer::setDataSource(const QString &source, bool reinitBackend)
{
    qDebug() << Q_FUNC_INFO << "source:" << source;

    mDataSource = source;
    switch (mPlaybackState) {
    case PlaybackState::Idle:
        if (reinitBackend) {
            initAndroidPlayer();
        }
        setPlaybackState(PlaybackState::Initialized);
        mAndroidPlayer.callMethod<void>("setDataSource",
                                        "(Ljava/lang/String;)V",
                                        QAndroidJniObject::fromString(source).object());
        {
            QAndroidJniEnvironment env;
            if (env->ExceptionCheck()) {
                env->ExceptionClear();
                emit error("setDataSource failed");
            } else {
                mAndroidPlayer.callMethod<void>("prepare");
            }
        }
        break;
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }
}

void AndroidMediaPlayer::pause()
{
    switch (mPlaybackState) {
    case PlaybackState::Started:
    case PlaybackState::Paused:
    case PlaybackState::PlaybackCompleted:
        setPlaybackState(PlaybackState::Paused);
        mAndroidPlayer.callMethod<void>("pause");
        break;
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }
}

void AndroidMediaPlayer::resume()
{
    setPlaybackState(PlaybackState::Started);
    mAndroidPlayer.callMethod<void>("resume");
}

void AndroidMediaPlayer::stop()
{
    switch (mPlaybackState) {
    case PlaybackState::Prepared:
    case PlaybackState::Started:
    case PlaybackState::Stopped:
    case PlaybackState::Paused:
    case PlaybackState::PlaybackCompleted:
        setPlaybackState(PlaybackState::Stopped);
        mAndroidPlayer.callMethod<void>("stop");
        break;
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }
}

void AndroidMediaPlayer::reset()
{
    switch (mPlaybackState) {
    case PlaybackState::Idle:
    case PlaybackState::Initialized:
    case PlaybackState::Prepared:
    case PlaybackState::Started:
    case PlaybackState::Paused:
    case PlaybackState::Stopped:
    case PlaybackState::PlaybackCompleted:
    case PlaybackState::Error:
        setPlaybackState(PlaybackState::Idle);
        mAndroidPlayer.callMethod<void>("reset");
        break;
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }

}

long AndroidMediaPlayer::currentPosition()
{
    switch (mPlaybackState) {
    case PlaybackState::Idle:
    case PlaybackState::Initialized:
    case PlaybackState::Prepared:
    case PlaybackState::Started:
    case PlaybackState::Paused:
    case PlaybackState::Stopped:
    case PlaybackState::PlaybackCompleted:
        return mAndroidPlayer.callMethod<jlong>("getCurrentPosition");
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }
    return 0;
}

long long AndroidMediaPlayer::duration()
{
    switch (mPlaybackState) {
    case PlaybackState::Prepared:
    case PlaybackState::Started:
    case PlaybackState::Paused:
    case PlaybackState::Stopped:
    case PlaybackState::PlaybackCompleted:
        return mAndroidPlayer.callMethod<jlong>("getDuration");
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }
    return 0;
}

void AndroidMediaPlayer::seekTo(long position)
{
    qDebug() << Q_FUNC_INFO;

    switch (mPlaybackState) {
    case PlaybackState::Prepared:
    case PlaybackState::Started:
    case PlaybackState::Paused:
    case PlaybackState::PlaybackCompleted:
        mAndroidPlayer.callMethod<void>("seekTo", "(J)V", jlong(position));
        break;
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }
}

void AndroidMediaPlayer::start()
{
    qDebug() << Q_FUNC_INFO;

    switch (mPlaybackState) {
    case PlaybackState::Prepared:
    case PlaybackState::Started:
    case PlaybackState::Paused:
    case PlaybackState::PlaybackCompleted:
        setPlaybackState(PlaybackState::Started);
        mAndroidPlayer.callMethod<void>("start", "()V");
        break;
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }
}

void AndroidMediaPlayer::setFillMode(AndroidMediaPlayer::VideoScalingMode mode)
{
    switch (mPlaybackState) {
    case PlaybackState::Initialized:
    case PlaybackState::Prepared:
    case PlaybackState::Started:
    case PlaybackState::Paused:
    case PlaybackState::Stopped:
    case PlaybackState::PlaybackCompleted:
        mAndroidPlayer.callMethod<void>("setVideoScalingMode",
                                        "(I)V",
                                        jint(mode));
        break;
    default:
        qWarning() << Q_FUNC_INFO << "player is in an invalid state: " << mPlaybackState;
        break;
    }
}

bool AndroidMediaPlayer::useRTPlayer() const
{
    return mUseRTPlayer;
}

bool AndroidMediaPlayer::autoStart() const
{
    return mAutoStart;
}

bool AndroidMediaPlayer::visible()
{
    if( mSurfaceView )
        return mSurfaceView->isVisible();
    return false;
}

QString AndroidMediaPlayer::getDataSource()
{
    return mDataSource;
}

void AndroidMediaPlayer::setSurfaceView(QQuickItem *surfaceView)
{
    qDebug() << Q_FUNC_INFO << surfaceView;

    if (mSurfaceView == surfaceView)
        return;

    disconnect(mSurfaceView, nullptr, this, nullptr);
    disconnect(this, nullptr, mSurfaceView, nullptr);
    disconnect(surfaceView, nullptr, this, nullptr);
    disconnect(this, nullptr, surfaceView, nullptr);

    mSurfaceView = surfaceView;
    QtAndroid::runOnAndroidThreadSync([this, surfaceView] {
        if (dynamic_cast<AndroidSurfaceView *>(surfaceView) != nullptr) {
            const auto asv = dynamic_cast<AndroidSurfaceView *>(surfaceView);
            connect(this, &AndroidMediaPlayer::videoSizeChanged,
                    asv, &AndroidSurfaceView::setVideoSize);
            connect(asv, &AndroidSurfaceView::surfaceChanged,
                    this, &AndroidMediaPlayer::setSurface);
            if (asv->surface().isValid()) {
                setSurface(asv->surface());
            }
        } else if (dynamic_cast<QSurfaceTexture *>(surfaceView)) {
            const auto qst = dynamic_cast<QSurfaceTexture *>(surfaceView);
            const auto onSurfaceTextureChanged = [this](QSurfaceTexture *surfaceTexture) {
                const auto &&surface = QAndroidJniObject("android/view/Surface",
                                                         "(Landroid/graphics/SurfaceTexture;)V",
                                                         surfaceTexture->surfaceTexture().object());
                setSurface(surface);
            };
            connect(qst, &QSurfaceTexture::surfaceTextureChanged,
                    this, onSurfaceTextureChanged);
            if (qst->surfaceTexture().isValid()) {
                onSurfaceTextureChanged(qst);
            }
        }
    });
    emit surfaceViewChanged(mSurfaceView.data());
}

void AndroidMediaPlayer::setUseRTPlayer(bool useRTPlayer)
{
    qDebug() << Q_FUNC_INFO << "callMethod useRTPlayer:" << useRTPlayer;
    mUseRTPlayer = useRTPlayer;
    mAndroidPlayer.callMethod<void>("useRTPlayer",
                                    "(Z)V",
                                    jboolean(useRTPlayer));
}

void AndroidMediaPlayer::setAutoStart(bool autoStart)
{
    mAutoStart = autoStart;
}

void AndroidMediaPlayer::onStarted()
{
    qDebug() << Q_FUNC_INFO;
    keepScreenOn(true);
}

void AndroidMediaPlayer::onFinished()
{
    qDebug() << Q_FUNC_INFO;
    setPlaybackState(PlaybackState::PlaybackCompleted);
    keepScreenOn(false);
}

void AndroidMediaPlayer::onBuffering(bool state)
{
    qDebug() << Q_FUNC_INFO << state;
    emit buffering(state);
}

void AndroidMediaPlayer::onError(int what, int extra)
{
    qDebug() << Q_FUNC_INFO << what << extra;

    QString msg;

    switch (extra) {
    case MEDIA_ERROR_UNKNOWN:
        msg = "Unknown media playback error.";
        break;
    case MEDIA_ERROR_SERVER_DIED:
        msg = "Connection to server lost.";
        break;
    default:
        msg = "Generic playback error.";
        break;
    }

    switch (what) {
    case MEDIA_ERROR_IO:
        msg = "IO media error.";
        break;
    case MEDIA_ERROR_MALFORMED:
        msg = "Malformed media.";
        break;
    case MEDIA_ERROR_UNSUPPORTED:
        msg = "Unsupported content type.";
        break;
    case MEDIA_ERROR_TIMED_OUT:
        msg = "Media timeout.";
        break;
    case MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK:
        msg = "Media not valid for progressive playback.";
        break;
    default:
        msg = "Other case of media playback error.";
        break;
    }
    setPlaybackState(PlaybackState::Error);
    emit error(msg);
}

void AndroidMediaPlayer::onPause()
{
    qDebug() << Q_FUNC_INFO;
    keepScreenOn(false);
    setPlaybackState(PlaybackState::Paused);
}

void AndroidMediaPlayer::onPrepared()
{
    qDebug() << Q_FUNC_INFO;
    setPlaybackState(PlaybackState::Prepared);
}

void AndroidMediaPlayer::onVideoSizeChanged(int width, int height)
{
    qDebug() << Q_FUNC_INFO;
    emit videoSizeChanged(width, height);
}

void AndroidMediaPlayer::setSurface(QAndroidJniObject surface) {
    qDebug() << Q_FUNC_INFO << mPlaybackState << "surface: " << surface.isValid();
    if (surface.isValid() && mPlaybackState != PlaybackState::Error) {
        mAndroidPlayer.callMethod<void>("setSurface",
                                        "(Landroid/view/Surface;)V",
                                        surface.object());
    }
}

void AndroidMediaPlayer::keepScreenOn(bool on) {
    QtAndroid::runOnAndroidThread([on]{
        const QAndroidJniObject &&activity = QtAndroid::androidActivity();
        if (activity.isValid()) {
            QAndroidJniObject &&window =
                activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

            if (window.isValid()) {
                const int FLAG_KEEP_SCREEN_ON = 128;
                if (on) {
                    window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                } else {
                    window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                }
            }
        }
        QAndroidJniEnvironment env;
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
    });
}

void AndroidMediaPlayer::setPlaybackState(PlaybackState newPlaybackState)
{
    qDebug() << Q_FUNC_INFO << "newPlaybackState:" << newPlaybackState;
    if (newPlaybackState != mPlaybackState) {
        mPlaybackState = newPlaybackState;
        emit playbackStateChanged(newPlaybackState);
    }
}

void AndroidMediaPlayer::initAndroidPlayer()
{
    qDebug() << Q_FUNC_INFO;
    if (mAndroidPlayer.isValid()) {
        release();
    }
    mAndroidPlayer = QAndroidJniObject("com/vadim/android/AndroidMediaPlayer");
    mPlaybackState = PlaybackState::Idle;
    mAndroidPlayer.callMethod<void>(std::get<0>(EVENT_LISTENER),
                                    std::get<1>(EVENT_LISTENER),
                                    QAndroidJniObject("com/vadim/android/NativeMediaPlayerEventListener",
                                                      "(J)V",
                                                      jlong(this)).object());
    const auto surfaceView = mSurfaceView;
    mSurfaceView = nullptr;
    setSurfaceView(surfaceView);
}

void AndroidMediaPlayer::release()
{
    setPlaybackState(PlaybackState::End);
    mAndroidPlayer.callMethod<void>("release");
}

void JNICALL Java_com_vadim_android_NativeMediaPlayerEventListener_onFinished
    (JNIEnv *, jclass, jlong listener) {
    QMetaObject::invokeMethod(reinterpret_cast<AndroidMediaPlayer*>(listener),
                              "onFinished", Qt::QueuedConnection);
}

void JNICALL Java_com_vadim_android_NativeMediaPlayerEventListener_onStarted
    (JNIEnv *, jclass, jlong listener) {
    QMetaObject::invokeMethod(reinterpret_cast<AndroidMediaPlayer*>(listener),
                              "onStarted", Qt::QueuedConnection);
}

void JNICALL Java_com_vadim_android_NativeMediaPlayerEventListener_onError
    (JNIEnv *, jclass, jlong listener, jint what, jint extra) {
    QMetaObject::invokeMethod(reinterpret_cast<AndroidMediaPlayer*>(listener),
                              "onError",
                              Qt::QueuedConnection, Q_ARG(int, what), Q_ARG(int, extra));
}

void JNICALL Java_com_vadim_android_NativeMediaPlayerEventListener_onBuffering
    (JNIEnv *, jclass, jlong listener, jboolean state) {
    QMetaObject::invokeMethod(reinterpret_cast<AndroidMediaPlayer*>(listener),
                              "onBuffering",
                              Qt::QueuedConnection, Q_ARG(bool, state));
}

void JNICALL Java_com_vadim_android_NativeMediaPlayerEventListener_onPause
    (JNIEnv *, jclass, jlong listener) {
    QMetaObject::invokeMethod(reinterpret_cast<AndroidMediaPlayer*>(listener),
                              "onPause", Qt::QueuedConnection);
}

void JNICALL Java_com_vadim_android_NativeMediaPlayerEventListener_onPrepared
    (JNIEnv *, jclass, jlong listener) {
    QMetaObject::invokeMethod(reinterpret_cast<AndroidMediaPlayer*>(listener),
                              "onPrepared", Qt::QueuedConnection);
}

void JNICALL Java_com_vadim_android_NativeMediaPlayerEventListener_onVideoSizeChanged(JNIEnv *,
                                                                                      jclass,
                                                                                      jlong listener,
                                                                                      jint width,
                                                                                      jint height) {
    QMetaObject::invokeMethod(reinterpret_cast<AndroidMediaPlayer*>(listener),
                              "onVideoSizeChanged", Qt::QueuedConnection,
                              Q_ARG(int, width), Q_ARG(int, height));
}
