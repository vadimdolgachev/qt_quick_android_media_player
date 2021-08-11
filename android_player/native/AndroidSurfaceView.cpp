#include "AndroidSurfaceView.h"

#include "com_vadim_android_NativeSurfaceChangeListener.h"

#include <QtAndroid>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

AndroidSurfaceView::AndroidSurfaceView(QQuickItem *parent) :
    QuickItemPlayerSurface(parent)
{
    qDebug() << Q_FUNC_INFO;

    QtAndroid::runOnAndroidThreadSync([this] {
        mSurfaceView = QAndroidJniObject("com/vadim/android/PlayerSurfaceView",
                                     "(Landroid/content/Context;)V",
                                     QtAndroid::androidContext().object());
        qDebug() << Q_FUNC_INFO << "AndroidSurfaceView" << mSurfaceView.isValid();
        mSurfaceView.callMethod<void>("setFocusable", "(Z)V", jboolean(false));
        mSurfaceView.callMethod<void>("setFocusableInTouchMode", "(Z)V", jboolean(false));
        mSurfaceView.callMethod<void>("setZOrderMediaOverlay", "(Z)V", jboolean(false));
        mSurfaceView.callMethod<void>("setZOrderOnTop", "(Z)V", jboolean(false));
        mSurfaceView.callMethod<void>("setSurfaceChangeListener",
                                      "(Lcom/vadim/android/SurfaceChangeListener;)V",
                                      QAndroidJniObject("com/vadim/android/NativeSurfaceChangeListener",
                                                        "(J)V",
                                                        jlong(this)).object());
    });
}

AndroidSurfaceView::~AndroidSurfaceView()
{
    qDebug() << Q_FUNC_INFO;
    QtAndroid::runOnAndroidThreadSync([this] {
        mSurfaceView.callMethod<void>("setSurfaceChangeListener",
                                      "(Lcom/vadim/android/SurfaceChangeListener;)V",
                                      nullptr);
    });
}

QAndroidJniObject AndroidSurfaceView::view() const
{
    return mSurfaceView;
}

QAndroidJniObject AndroidSurfaceView::surface() const
{
    return mSurface;
}

AndroidSurfaceView::ScalingMode AndroidSurfaceView::scalingMode() const
{
    return mScalingMode;
}

void AndroidSurfaceView::setVideoSize(int width, int height)
{
    qDebug() << Q_FUNC_INFO << width << height;
    QtAndroid::runOnAndroidThread([this, width, height] {
        mSurfaceView.callMethod<void>("setVideoSize",
                                      "(II)V",
                                      jint(width), jint(height));
    });
}

void AndroidSurfaceView::onSurfaceChanged(QAndroidJniObject surface)
{
    qDebug() << Q_FUNC_INFO;
    if (mSurface != surface) {
        mSurface = surface;
        emit surfaceChanged(surface);
    }
}

void AndroidSurfaceView::setScalingMode(AndroidSurfaceView::ScalingMode scalingMode)
{
    qDebug() << Q_FUNC_INFO << "scalingMode:" << scalingMode;

    if (mScalingMode == scalingMode)
        return;
    mScalingMode = scalingMode;
    QtAndroid::runOnAndroidThread([this, scalingMode] {
        mSurfaceView.callMethod<void>("setScalingMode",
                                      "(I)V",
                                      jint(scalingMode));
    });
    emit scalingModeChanged(mScalingMode);
}

JNIEXPORT void JNICALL Java_com_vadim_android_NativeSurfaceChangeListener_onSurfaceChanged
(JNIEnv *, jclass, jlong listener, jobject surface) {
    qDebug() << Q_FUNC_INFO << "listener:" << reinterpret_cast<AndroidSurfaceView *>(listener)
             << "surface:" << surface;
    QMetaObject::invokeMethod(reinterpret_cast<AndroidSurfaceView *>(listener),
                              "onSurfaceChanged", Qt::QueuedConnection,
                              Q_ARG(QAndroidJniObject, QAndroidJniObject(surface)));
}
