import QtQuick 2.0
import mur 1.0


Rectangle {
    id: remoteView;
    property var controller: Controllers.image;
    height: parent.height;
    width: parent.width;
    color: Style.bgBlue;
    property bool shrink: width < 300;

    Rectangle {

        id: remoteHeader;
        height: 0;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        color: Style.bgDark;
    }

    Rectangle {
        id: spearator;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: remoteHeader.bottom;
        height: 1;
        color: Style.bgDarker;
    }

    Column {
        id: imageView;
        anchors.left: parent.left;
        anchors.top: spearator.bottom;
        anchors.right: parent.right;
        anchors.bottom: remoteFooter.top;

        Rectangle {
            id: front;
            implicitHeight: Controllers.network.usv ? parent.height : parent.height / 2;
            implicitWidth: parent.width;

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    controller.saveImageFront();
                }
            }

            ImageItem {
              id: frontImage;
              height: front.height
              width: front.width
              image: controller.front;
            }
        }

        Rectangle {
            id: bottom;
            visible: !Controllers.network.usv;
            implicitHeight: parent.height / 2;
            implicitWidth: parent.width;

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    controller.saveImageBottom();
                }
            }

            ImageItem {
              id: bottomImage;
              height: bottom.height;
              width: bottom.width;
              image: controller.bottom;
            }
        }
    }

    Rectangle {
        id: remoteFooter;
        height: 22;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        color: Style.bgDark;

        Row {
            id: fileNameRow;

            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.left: parent.left;
            anchors.leftMargin: 8;

            spacing: 4;

            Icon {
                anchors.verticalCenter: parent.verticalCenter;
                icon: icons.fa_gamepad;
                color: controller.Gamepad.Gamepad.connected ? Style.green : Style.red;
            }

            UiLabel {
                visible: !remoteView.shrink;
                anchors.verticalCenter: parent.verticalCenter;
                text: remoteView.width < 350 ? "Gamepad: " : "Gamepad axes: ";
                font.pointSize: 10;
            }

            UiLabel {
                font.family: Style.fontMono;
                anchors.verticalCenter: parent.verticalCenter;
                property var axisX: controller.Gamepad.axisXValue;
                property var axisY: controller.Gamepad.axisYValue;
                property var axisZ: controller.Gamepad.axisZValue;

                text: axisX +":"+axisY +":"+axisZ;
                font.pointSize: 10;
            }
        }

        Row {
            anchors.right: parent.right;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.rightMargin: 4;
            spacing: 4;
            visible: remoteView.width > 140;

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                highlight: controller.autoYaw;
                label.text: remoteView.shrink ? "" : "Auto Yaw";
                toolTip: "Auto Yaw regulator";
                icon: remoteView.shrink ? icons.fa_compass : null;
                onClicked: {
                    controller.autoYaw = !controller.autoYaw;
                }

            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                highlight: controller.autoDepth;
                label.text: remoteView.shrink ? "" : "Auto Depth";
                toolTip: "Auto Depth regulator";
                icon: remoteView.shrink ? icons.fa_arrows_v : null;
                onClicked: {
                    controller.autoDepth = !controller.autoDepth;
                }
            }
        }
    }

    Rectangle {
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: remoteFooter.top;
        height: 1;
        color: Style.bgDarker;
    }
}
