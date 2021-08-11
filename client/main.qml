import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import com.vadim.android 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 600
    title: qsTr("MediaPlayer")
    opacity: 0
    color: "#00000000"

    property var videos: [
        "/storage/emulated/0/Download/big_buck_bunny_360p_2mb.mp4",
        "/storage/69D8-6834/Download/Kuzmin_01.mkv",
        "/storage/69D8-6834/Download/Casino32.mkv",
    ]

    MouseArea {
        anchors.fill: parent
        onClicked: {
            switch(player2.surfaceView.scalingMode) {
            case AndroidSurfaceView.ScalingToFitMode:
                player2.surfaceView.scalingMode = AndroidSurfaceView.ScalingToFillMode
                return
            case AndroidSurfaceView.ScalingToFillMode:
                player2.surfaceView.scalingMode = AndroidSurfaceView.ScalingPanAndScanMode
                return
            case AndroidSurfaceView.ScalingPanAndScanMode:
                player2.surfaceView.scalingMode = AndroidSurfaceView.ScalingToFitMode
                return
            }
        }
    }

    AndroidMediaPlayer {
        id: player2
        useRTPlayer: true
        autoStart: true

        surfaceView: AndroidSurfaceView {
            scalingMode: AndroidSurfaceView.ScalingToFitMode
            anchors.fill: parent
        }

        onPlaybackStateChanged: {
            print("playbackState: ", playbackState)
            switch(playbackState) {
            case AndroidMediaPlayer.Prepared:
                player2.start()
                break;
            case AndroidMediaPlayer.PlaybackCompleted:
                current_index += 1
                if (player2.current_index >= videos.length) {
                    player2.current_index = 0
                }
                player2.stop()
                player2.reset()
                player2.setDataSource(videos[player2.current_index], true)
                break;
            }
        }

        property int current_index: 0
        Component.onCompleted: {
            player2.setDataSource(videos[player2.current_index])
        }
    }

    SurfaceTexture {
        id: surfaceTexture3
        x: window.width / 2
        height: window.height / 2
        width: window.width / 2
        layer.enabled: true
        layer.effect: FakeAlpha {}
    }

    AndroidMediaPlayer {
        id: player3
        useRTPlayer: false
        autoStart: false
        surfaceView: surfaceTexture3

        onPlaybackStateChanged: {
            print("playbackState: ", playbackState)
            switch(playbackState) {
            case 0:
                break;
            case 1:
                if(autoStart) player3.play()
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                seek(0, 0)
                timer1.start()
                break;
            }
        }

        property int current_index: 0
    }
}
