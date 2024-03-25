import QtQuick 2.12
import QtQuick.Layouts 1.12
import mur 1.0

ColumnLayout {
    id: cam;
    property alias image: cameraImage.image;
    property bool haveImage: false;
    property bool isFullscreen: false;
    property var originalParent: parent;
    property string subtitle: "";
    function saveImage() {}

    Item {
        parent: isFullscreen ? fullWindowContainer : cam;
        Layout.fillWidth: true;
        Layout.fillHeight: true;

        ImageItem {
            id: cameraImage;
            anchors.fill: parent;

            height: root.height;
            width: root.width;
        }

        UiLabel {
            id: placeholder;
            anchors.fill: parent;
            verticalAlignment: Text.AlignVCenter;
            horizontalAlignment: Text.AlignHCenter;
            text: "<h1 style=\"margin: 0; padding: 0;\">Video</h1>" + subtitle;
            textFormat: Text.RichText;
            opacity: width > 150 && !haveImage ? 1.0 : 0.0;
            width: root.width;
            clip: true;

            Behavior on opacity {
                NumberAnimation {
                    duration: Style.animFast;
                    easing.type: Style.animEasing;
                }
            }
        }

        MouseArea {
            id: mouseArea;
            anchors.fill: parent;
            hoverEnabled: true;
            onClicked: {
                if (isFullscreen) return;

                if (haveImage) {
                    saveImage();
                    notifications.notifyImageSaved();
                } else {
                    notifications.notifyEmptyImage();
                }
            }
        }

        UiButton {
            id: buttonFulscreeenCamera;
            icon: cam.isFullscreen ? icons.fa_compress : icons.fa_expand;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            onClicked: {
                cam.isFullscreen = !cam.isFullscreen;
                fullWindow.visible = cam.isFullscreen;
                if (!cam.isFullscreen) remoteToolbar.collapsed = false;
                root.visibility = cam.isFullscreen ? "FullScreen" : "Windowed";
            }
            frameless: true;
            outline: true;
            highlight: cam.isFullscreen;
            width: height;
            height: 32;
            opacity: ma.containsMouse ? 1.0 : mouseArea.containsMouse ? 0.5 : 0.0;
        }

        Connections {
            target: cameraImage;
            onImageChanged: function () {
                haveImage = true;
            }
        }
    }
}

