import QtQuick 2.12
import QtQuick.Layouts 1.12
import mur 1.0


Rectangle {
    id: compass;
    property real targetYaw: Controllers.remote.targetYaw + 180;
    property real rawYaw: Controllers.network.yaw + 180;
    property real yaw: -(rawYaw - 180) + 90 - (360 / 1.5);

    Behavior on rawYaw {
        RotationAnimation {
            id: yawAnim;
            alwaysRunToEnd: true;
            duration: 200;
            easing.type: Easing.Linear;
            direction: RotationAnimation.Shortest;
       }
    }

    Behavior on targetYaw {
        RotationAnimation {
            id: targetYawAnim;
            alwaysRunToEnd: true;
            duration: 50;
            easing.type: Easing.Linear;
            direction: RotationAnimation.Shortest;
       }
    }

    anchors.top: parent.top;
    anchors.topMargin: 60;
    width: Math.min(parent.width, 1000);
    anchors.horizontalCenter: parent.horizontalCenter;
    height: 40;
    clip: true;
    color: "transparent";
    border.width: 1;
    border.color: "#70000000";

    Rectangle {
        id: meter;
        property real parts: 8;
        property real strokes: 4 * 16;
        property real w: parent.width * 1.5;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        width: parent.width;
        color: Style.outlineColor;

        Item {
            anchors.fill: parent;

            Repeater {
                model: meter.strokes;

                Rectangle {
                    x: (((compass.yaw + 360) * (meter.w / 360)) + (index * meter.w / meter.strokes)) % meter.w;
                    height: 2 + parent.height / 4 * ((index + 3) % 2 * (index + 3) % 4);
                    anchors.verticalCenter: parent.verticalCenter;

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        width: 2;
                        height: parent.height;
                        color: "#FFFFFF";
                        opacity: 0.4;
                    }
                }
            }

            Repeater {
                model: meter.parts * 2;

                Rectangle {
                    x: (((compass.yaw + 360) * (meter.w / 360)) + ((index + 2) * meter.w / meter.parts)) % (meter.w * 2) - meter.w;
                    height: parent.height;
                    anchors.verticalCenter: parent.verticalCenter;

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        width: meter.width / 10;
                        height: parent.height;
                        opacity: index % 2 === 0 ? 0.6 : 0.3;

                        gradient: Gradient {
                            orientation: Gradient.Horizontal;
                            GradientStop { position: 0.0; color: "#00FFFFFF" }
                            GradientStop { position: 0.5; color: "#CCAAAAAA" }
                            GradientStop { position: 1.0; color: "#00FFFFFF" }
                        }
                    }

                    UiLabel {
                        property bool big: index % 2 === 0;
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        property int degree: (360 / meter.parts * (index) - 180);
                        text: degree <= -180 ? degree + 360 : degree > 180 ? degree - 360 : degree;
                        font.pointSize: big ? 12 : 9;
                        font.bold: true;
                        color: "#FFFFFF";
                        opacity: big ? 0.8 : 0.4;
                        style: Text.Outline;
                        styleColor: Style.outlineColor;
                    }
                }
            }
        }

        Icon {
            x: (((compass.yaw + 450 + compass.targetYaw) * (meter.w / 360)) % meter.w) - (width / 2);
            anchors.top: meter.top;
            anchors.topMargin: -7;
            visible: Controllers.remote.autoYawAltmode;
            icon: icons.fa_caret_down;
            font.pointSize: 16;
            color: "#BBAAFFAA";
            style: Text.Outline;
            styleColor: Style.outlineColor;
        }
    }

    Icon {
        anchors.bottom: compass.bottom;
        anchors.bottomMargin: -5;
        anchors.horizontalCenter: compass.horizontalCenter;
        icon: icons.fa_caret_up;
        font.pointSize: 16;
        color: Controllers.remote.speedMode === 0 ? Style.gray : Controllers.remote.speedMode === 2 ? Style.yellow : Style.white;
        style: Text.Outline;
        styleColor: Style.outlineColor;
    }

    UiLabel {
        anchors.topMargin: -2;
        anchors.top: compass.top;
        anchors.bottom: compass.bottom;
        anchors.horizontalCenter: compass.horizontalCenter;
        font.bold: true;
        text: Controllers.network.yaw.toFixed(0);
        color: "white";
        style: Text.Outline;
        styleColor: Style.outlineColor;
    }
}
