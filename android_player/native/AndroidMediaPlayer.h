#ifndef PLAYER_H
#define PLAYER_H

#include <QAndroidJniObject>
#include <QObject>
#include <QPointer>
#include <QQuickItem>
#include <QMetaType>

class AndroidSurfaceView;
class QQuickItem;

class AndroidMediaPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *surfaceView READ surfaceView WRITE setSurfaceView NOTIFY surfaceViewChanged)
    Q_PROPERTY(PlaybackState playbackState READ playbackState NOTIFY playbackStateChanged)
    Q_PROPERTY(bool useRTPlayer READ useRTPlayer WRITE setUseRTPlayer NOTIFY useRTPlayerChanged)
    Q_PROPERTY(bool autoStart READ autoStart WRITE setAutoStart)

public:
    AndroidMediaPlayer(QObject *parent = nullptr);
    ~AndroidMediaPlayer();

    enum VideoScalingMode {
        ScaleToFit = 1,
        ScaleToFitWithCropping = 2
    };
    Q_ENUM(VideoScalingMode)

    enum class PlaybackState {
        Idle,
        Initialized,
        Preparing,
        Prepared,
        Started,
        Stopped,
        Paused,
        PlaybackCompleted,
        Error,
        End
    };
    Q_ENUM(PlaybackState)

    QString getDataSource();
    Q_INVOKABLE void setDataSource(const QString &source, bool reinitBackend = false);
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void reset();
    Q_INVOKABLE long currentPosition();
    Q_INVOKABLE long long duration();
    Q_INVOKABLE void seekTo(long position);
    Q_INVOKABLE void start();
    QQuickItem *surfaceView() const;
    PlaybackState playbackState() const;
    void setFillMode(VideoScalingMode mode);
    bool useRTPlayer() const;
    bool autoStart() const;
    bool visible();

signals:
    void playbackStateChanged(PlaybackState playbackState);
    void error(QString error);
    void buffering(bool state);
    void surfaceViewChanged(QQuickItem * surfaceView);
    void videoSizeChanged(int width, int height);
    void useRTPlayerChanged(bool useRTPlayer);

public slots:
    void setSurfaceView(QQuickItem *surfaceView);
    void setUseRTPlayer(bool useRTPlayer);
    void setAutoStart(bool autoStart);

private slots:
    void onStarted();
    void onFinished();
    void onBuffering(bool state);
    void onError(int what, int extra);
    void onPause();
    void onPrepared();
    void onVideoSizeChanged(int width, int height);
    void setSurface(QAndroidJniObject surfaceView);

private:
    void keepScreenOn(bool on);
    void setPlaybackState(PlaybackState newPlaybackState);
    void initAndroidPlayer();
    void release();

    QPointer<QQuickItem> mSurfaceView;
    PlaybackState mPlaybackState;
    QAndroidJniObject mAndroidPlayer;
    bool mUseRTPlayer;
    bool mAutoStart;
    QString mDataSource;
};

Q_DECLARE_METATYPE(AndroidMediaPlayer::PlaybackState)

#endif // PLAYER_H
