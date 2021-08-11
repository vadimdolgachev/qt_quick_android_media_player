#include "QuickItemSurface.h"

#include <QGuiApplication>
#include <QScreen>
#include <QtAndroid>

QuickItemPlayerSurface::QuickItemPlayerSurface(QQuickItem *parent) :
    QQuickItem (parent)
{
    connect(this, &QQuickItem::xChanged, this, &QuickItemPlayerSurface::onGeometyChanged);
    connect(this, &QQuickItem::yChanged, this, &QuickItemPlayerSurface::onGeometyChanged);
    connect(this, &QQuickItem::widthChanged, this, &QuickItemPlayerSurface::onGeometyChanged);
    connect(this, &QQuickItem::heightChanged, this, &QuickItemPlayerSurface::onGeometyChanged);
}

QRect QuickItemPlayerSurface::toPhycalGeometry(qreal x, qreal y, qreal w, qreal h)
{
    const auto ratio = qApp->primaryScreen()->devicePixelRatio();
    const auto physicalX = qRound(x * ratio);
    const auto physicalY = qRound(y * ratio);
    const auto physicalWidth = qRound(w * ratio);
    const auto physicalHeight = qRound(h * ratio);

    return {physicalX, physicalY, physicalWidth, physicalHeight};
}

void QuickItemPlayerSurface::onGeometyChanged()
{
    QtAndroid::runOnAndroidThreadSync([this] {
        const auto &&rect = toPhycalGeometry(x(), y(), width(), height());
        QtAndroid::androidActivity().callMethod<void>("setPlayerSurfaceGeometry",
                                                      "(Landroid/view/View;IIII)V",
                                                      jobject(view().object()),
                                                      jint(rect.x()), jint(rect.y()),
                                                      jint(rect.width()), jint(rect.height()));
    });
}

void QuickItemPlayerSurface::componentComplete()
{
    QQuickItem::componentComplete();

    QtAndroid::runOnAndroidThreadSync([this] {
        const auto &&rect = toPhycalGeometry(x(), y(), width(), height());
        QtAndroid::androidActivity().callMethod<void>("addPlayerSurface",
                                                      "(Landroid/view/View;IIII)V",
                                                      jobject(view().object()),
                                                      jint(rect.x()), jint(rect.y()),
                                                      jint(rect.width()), jint(rect.height()));
    });
}
