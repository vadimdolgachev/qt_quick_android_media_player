#ifndef QSURFACETEXTURE_H
#define QSURFACETEXTURE_H

#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QQuickItem>

class QSurfaceTexture : public QQuickItem
{
    Q_OBJECT
public:
    QSurfaceTexture(QQuickItem *parent = nullptr);
    ~QSurfaceTexture();

    // returns surfaceTexture Java object.
    const QAndroidJniObject &surfaceTexture() const;

    // QQuickItem interface
protected:
    QSGNode *updatePaintNode(QSGNode *n, UpdatePaintNodeData *) override;

signals:
    void surfaceTextureChanged(QSurfaceTexture *surfaceTexture);

private:
    // our texture
    uint32_t mTextureId = 0;

    // Java SurfaceTexture object
    QAndroidJniObject mSurfaceTexture;
};

#endif // QSURFACETEXTURE_H
