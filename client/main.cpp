#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <native/AndroidSurfaceView.h>
#include <native/QSurfaceTexture.h>
#include <native/AndroidMediaPlayer.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<AndroidMediaPlayer>("com.vadim.android", 1, 0, "AndroidMediaPlayer");
    qmlRegisterType<AndroidSurfaceView>("com.vadim.android", 1, 0, "AndroidSurfaceView");
    qmlRegisterType<QSurfaceTexture>("com.vadim.android", 1, 0, "SurfaceTexture");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
