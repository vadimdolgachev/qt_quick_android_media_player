#ifndef SURFACE_H
#define SURFACE_H

#include "QuickItemSurface.h"

#include <QAndroidJniObject>

class AndroidSurfaceView : public QuickItemPlayerSurface
{
    Q_OBJECT
    Q_PROPERTY(ScalingMode scalingMode READ scalingMode WRITE setScalingMode NOTIFY scalingModeChanged)
public:
    AndroidSurfaceView(QQuickItem *parent = nullptr);
    ~AndroidSurfaceView() override;

    enum ScalingMode {
        ScalingToFillMode = 0,
        ScalingToFitMode = 1,
        ScalingPanAndScanMode = 2
    };
    Q_ENUM(ScalingMode)

    QAndroidJniObject view() const override;
    QAndroidJniObject surface() const;
    ScalingMode scalingMode() const;

signals:
    void surfaceChanged(QAndroidJniObject surface);
    void scalingModeChanged(ScalingMode scalingMode);

public slots:
    void setVideoSize(int width, int height) override;
    void onSurfaceChanged(QAndroidJniObject surface);
    void setScalingMode(ScalingMode scalingMode);

private:
    QAndroidJniObject mSurfaceView;
    QAndroidJniObject mSurface;
    ScalingMode mScalingMode;
};

Q_DECLARE_METATYPE(QAndroidJniObject)

#endif // SURFACE_H
