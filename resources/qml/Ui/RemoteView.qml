import QtQuick 2.0
import mur 1.0

Rectangle {
    id: remoteView;
    property var controller: Controllers.image;
    height: parent.height;
    width: parent.width;
    color: "#282C34";

    Rectangle {

        id: remoteHeader;
        height: 22;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        color: "#21252B";


//        Row {

//            anchors.left: parent.left;
//            anchors.top: parent.top;
//            anchors.right: parent.right;
//            anchors.bottom: parent.bottom;
//            anchors.leftMargin: 4;
//            spacing: 4;

//            UiButton {
//                anchors.verticalCenter: parent.verticalCenter;
//                frameless: true;
//                toolTip: "Run code remote/local";
//               // label.text: Controllers.scripts.local ? "Local" : "Remote";
//               // highlight: Controllers.scripts.local;
//                label.text: "Start/Stop remote mode";
//                icon: icons.fa_desktop;
//                onClicked: {
//                    controller.on_target_mode_changed();
//                }
//            }

//            UiButton {
//                anchors.verticalCenter: parent.verticalCenter;
//                frameless: true;
//                toolTip: "Run code remote/local";
//               // label.text: Controllers.scripts.local ? "Local" : "Remote";
//               // highlight: Controllers.scripts.local;
//                label.text: "Start/Stop remote mode";
//                icon: icons.fa_desktop;
//                onClicked: {
//                    controller.on_target_mode_changed();
//                }
//            }
//        }
    }

    Rectangle {
        id: spearator;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: remoteHeader.bottom;
        height: 1;
        color: "#181A1F";
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
            implicitWidth:  parent.width;

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
        color: "#21252B";

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
                color: controller.Gamepad.Gamepad.connected ? "#148F77" : "#E74C3C";
            }

            UiLabel {
                anchors.verticalCenter: parent.verticalCenter;
                text: "Gamepad axes: ";
                font.pointSize: 10;
            }

            UiLabel {
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
            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: controller.autoYaw;
                label.text: "Auto Yaw";
                visible: remoteView.width > 275;
                onClicked: {
                    controller.autoYaw = !controller.autoYaw;
                }

            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: controller.autoDepth;
                label.text: "Auto Depth";
                visible: remoteView.width > 275;
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
        color: "#181A1F";
    }
}
