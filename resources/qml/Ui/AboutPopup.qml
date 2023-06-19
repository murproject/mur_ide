import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4

Popup {
    id: aboutPopup;
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    bottomPadding: 24;
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside;
    modal: true;
    visible: true;
    parent: ApplicationWindow.overlay;

    Overlay.modal: Rectangle {
        color: Style.overlayBack;
        Behavior on opacity {
           NumberAnimation {
               duration: Style.animFastest
           }
       }
    }

    background: Rectangle {
        anchors.fill: parent;
        color: Style.bgDark;
        border.width: 1;
        border.color: Style.bgDarker;
    }

    Row {
        anchors.centerIn: parent

        Column {
        topPadding: 8;
            Image {
                id: murLogo;
                source: "qrc:/images/mur_logo256.png"
                width: 64;
                height: 64
                horizontalAlignment: Image.AlignLeft
                verticalAlignment: Image.AlignTop
            }
        }

        Column {
            topPadding: 8;
            leftPadding: 8;
            Text {
               color: "#F39C12";
               font.underline: false;
               linkColor: color;
               width: Math.min(220, contentWidth)
               text: qsTr("<a href=\"https://robocenter.net/\">The center for  robotics development LLC.</a>")
               wrapMode: Text.WordWrap
               horizontalAlignment: Text.AlignHCenter + 5;
               onLinkActivated: Qt.openUrlExternally(link);
            }
            Text {
               anchors.topMargin: 8;
               anchors.leftMargin: 32;
               width: Math.min(200, contentWidth)
               font.family: Style.fontSans;
               color: Style.lighterGray;
               linkColor: Style.lightBlue;
               text: qsTr("<h2>MUR IDE v" + Controllers.version + "</h2>
                           <a href=\"https://github.com/murproject/mur_ide\">Source code</a>
                           (MIT license)
                           <br><br>Powered by:<br><a href=\"https://www.qt.io/\">Qt</a>,
                           <a href=\"https://opencv.org/\">OpenCV</a>,
                           <a href=\"https://gstreamer.freedesktop.org/\">GStreamer</a>
                           and <a href=\"https://www.python.org/\">Python</a>
                           <br><br>
                           Contact us: <a href=\"mailto:support@robocenter.org\">support@robocenter.org</a>
                           ");
               wrapMode: Text.WordWrap
               onLinkActivated: Qt.openUrlExternally(link);
            }
        }
    }

    enter: Transition {
        NumberAnimation {
            property: "opacity";
            from: 0.0;
            to: 1.0;
            easing.type: Style.animEasing;
            duration: Style.animDuration;
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "opacity";
            from: 1.0;
            to: 0.0;
            easing.type: Style.animEasing;
            duration: Style.animDuration;
        }
    }
}
