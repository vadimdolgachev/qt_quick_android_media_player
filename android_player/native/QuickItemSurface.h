#ifndef QUICKITEMSURFACE_H
#define QUICKITEMSURFACE_H

#include <QAndroidJniObject>
#include <QQuickItem>


class QuickItemPlayerSurface : public QQuickItem
{
    Q_OBJECT
public:
    QuickItemPlayerSurface(QQuickItem *parent = nullptr);

    virtual QAndroidJniObject view() const = 0;

public slots:
    Q_INVOKABLE virtual void setVideoSize(int width, int height) = 0;

private slots:
    void onGeometyChanged();

protected:
    void componentComplete() override;

protected:
    static QRect toPhycalGeometry(qreal x, qreal y, qreal w, qreal h);
};

#endif // QUICKITEMSURFACE_H
