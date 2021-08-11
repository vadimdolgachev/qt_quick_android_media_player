#-------------------------------------------------
#
# Project created by QtCreator 2019-02-08T20:39:58
#
#-------------------------------------------------

QT       += qml quick
QT       += androidextras

TARGET = android_player
TEMPLATE = lib
CONFIG += staticlib c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    native/AndroidMediaPlayer.cpp \
    native/AndroidSurfaceView.cpp \
    native/QuickItemSurface.cpp \
    native/QSurfaceTexture.cpp

HEADERS += \
    native/AndroidMediaPlayer.h \
    native/com_vadim_android_NativeMediaPlayerEventListener.h \
    native/com_vadim_android_NativeSurfaceChangeListener.h \
    native/AndroidSurfaceView.h \
    native/QuickItemSurface.h \
    native/QSurfaceTexture.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/local.properties \
    android/src/com/vadim/android/AndroidMediaPlayer.java \
    android/src/com/vadim/android/IPlayerSurfaceActivity.java \
    android/src/com/vadim/android/PlayerSurfaceActivity.java \
    android/src/com/vadim/android/SurfaceTextureListener.java \
    android/src/com/vadim/android/PlayerSurfaceView.java \
    android/src/com/vadim/android/SurfaceChangeListener.java \
    android/src/com/vadim/android/NativeSurfaceChangeListener.java \
    android/src/com/vadim/android/MediaPlayerEventListener.java \
    android/src/com/vadim/android/NativeMediaPlayerEventListener.java

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

AarLibTarget.target = AarLib
AarLibTarget.depends = FORCE
AarLibTarget.commands = cd $$PWD/android && bash gradlew assembleRelease -PbuildDir=$$OUT_PWD/aar
PRE_TARGETDEPS += AarLib
QMAKE_EXTRA_TARGETS += AarLibTarget
