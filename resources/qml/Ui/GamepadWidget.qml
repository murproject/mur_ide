import QtQml 2.2
import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import mur.GamepadAxes 1.0

ColumnLayout {
    id: gamepadWidget;
    property real widgetWidth: 100;
    property real widgetHeight: 100;
    property bool showIndicator: true;
    property bool showValues: true;
    property bool opaqueWhenConnected: true;

    RowLayout {
        Layout.alignment: Qt.AlignHCenter;
        visible: gamepadWidget.showIndicator;

        Icon {
            style: Text.Outline;
            styleColor: Style.outlineColor;
            Layout.alignment: Qt.AlignHCenter;
            icon: icons.fa_gamepad;
            color: Controllers.joystick.joystickConnected ? Style.green : Style.red;
        }

        UiLabel {
            Layout.alignment: Qt.AlignHCenter;
            text: Controllers.joystick.joystickConnected ? "Connected" : "Not connected";
        }

    }


    RowLayout {
        Layout.alignment: Qt.AlignHCenter;
        Layout.fillWidth: true;
        opacity: Controllers.joystick.joystickConnected || !opaqueWhenConnected ? 1.0 : 0.3;

        Item {
            property real divider: 200 / gamepadWidget.widgetWidth;
            width: gamepadWidget.widgetWidth;
            height: gamepadWidget.widgetHeight;

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                color: "transparent";
                border.width: 4;
                border.color: "#40FFFFFF";
                radius: 999;
                height: parent.height;
                width: height;
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                color: "transparent";
                border.width: 4;
                border.color: "#40FFFFFF";
                radius: 999;
                height: parent.height / 5;
                width: parent.width / 5;
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.verticalCenterOffset: Controllers.joystick.allAxes[GamepadAxes.AxisY] / parent.divider;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.horizontalCenterOffset: Controllers.joystick.allAxes[GamepadAxes.AxisX] / parent.divider;
                color: "white";
                radius: 999;
                height: parent.height / 7;
                width: parent.width / 7;
            }
        }

        Item {
            property real divider: 200 / gamepadWidget.widgetWidth;
            width: gamepadWidget.widgetWidth;
            height: gamepadWidget.widgetHeight;

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                color: "transparent";
                border.width: 4;
                border.color: "#40FFFFFF";
                radius: 999;
                height: parent.height;
                width: height;
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                color: "transparent";
                border.width: 4;
                border.color: "#40FFFFFF";
                radius: 999;
                height: parent.height / 5;
                width: parent.width / 5;
            }

            Rectangle {
                property int axis: 0;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.verticalCenterOffset: Controllers.joystick.allAxes[GamepadAxes.AxisZ] / parent.divider ;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.horizontalCenterOffset: Controllers.joystick.allAxes[GamepadAxes.AxisW] / parent.divider;
                color: "white";
                radius: 999;
                height: parent.height / 7;
                width: parent.width / 7;
            }
        }
    }

    Row {
        Layout.alignment: Qt.AlignHCenter;
        visible: gamepadWidget.showValues;

        Repeater {
            model: [
                Controllers.joystick.allAxes[GamepadAxes.AxisX],
                Controllers.joystick.allAxes[GamepadAxes.AxisY],
                Controllers.joystick.allAxes[GamepadAxes.AxisZ],
                Controllers.joystick.allAxes[GamepadAxes.AxisW],
            ]

            UiLabel {
                width: 50;
                horizontalAlignment: Text.AlignHCenter;
                text: modelData;
                font.family: Style.fontMono;
            }
        }
    }
}
