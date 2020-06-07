import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4

Popup {
    id: aboutPopup;
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside;
    modal: true;
    visible: true;
    parent: ApplicationWindow.overlay;


    background: Rectangle {
        anchors.fill: parent;
        color: "#21252B";
        border.width: 1;
        border.color: "#181A1F";
    }

    Row {
        anchors.centerIn: parent

        Image {
            id: murLogo;
            source: "qrc:/images/mur_logo256.png"
            width: 64;
            height: 64
            horizontalAlignment: Image.AlignLeft
            verticalAlignment: Image.AlignTop
        }

        Column {
            topPadding: 8;
            leftPadding: 8;
            Text {
               color: "#F39C12";
               width: Math.min(220, contentWidth)
               text: qsTr("The center for  robotics development LLC.")
               wrapMode: Text.WordWrap
               horizontalAlignment: Text.AlignHCenter + 5;
            }
            Text {
               anchors.topMargin: 8;
               anchors.leftMargin: 32;

               color: "#2B68A4";
               width: Math.min(200, contentWidth)
               text: qsTr("murIDE v0.0.8. Powered by: <a href=\"https://www.qt.io/\">Qt</a>, <a href=\"https://opencv.org/\">OpenCV</a>,
                           <a href=\"https://gstreamer.freedesktop.org/\">GStreamer</a> and <a href=\"https://www.python.org/\">Python</a>");
               wrapMode: Text.WordWrap
               onLinkActivated: Qt.openUrlExternally(link);
            }
        }
    }
}
