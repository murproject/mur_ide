import QtQml 2.2
import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import mur.GamepadAxes 1.0
import input.KeyboardAxes 1.0

ColumnLayout {
    id: gamepadWidget;
    property real widgetWidth: 100;
    property real widgetHeight: 100;
    property bool showIndicator: true;
    property bool showValues: true;
    property bool opaqueWhenConnected: true;
    property bool isKeyboard: false;


    function calculateOffset(index1, index2) {
        var value1 = Controllers.joystick.allAxes[index1];
        var value2 = Controllers.joystick.allAxes[index2];
        return (value1 > value2) ? -value1 : value2;
    }

    function calculateOffsetKeyboard(index1, index2){
        var value1 = Controllers.keyboard.allAxes[index1] * 100;
        var value2 = Controllers.keyboard.allAxes[index2] * 100;
        return (Math.abs(value1) > Math.abs(value2)) ? value1 : value2;
    }

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
                anchors.verticalCenterOffset: (isKeyboard ? calculateOffsetKeyboard(KeyboardAxes.AxisYm, KeyboardAxes.AxisYp) :
                                                            calculateOffset(GamepadAxes.AxisYm, GamepadAxes.AxisYp)) / parent.divider;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.horizontalCenterOffset: (isKeyboard ? calculateOffsetKeyboard(KeyboardAxes.AxisXm, KeyboardAxes.AxisXp) :
                                                              calculateOffset(GamepadAxes.AxisXm, GamepadAxes.AxisXp)) / parent.divider;
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
                anchors.verticalCenterOffset: (isKeyboard ? calculateOffsetKeyboard(KeyboardAxes.AxisZm, KeyboardAxes.AxisZp)
                                                          : calculateOffset(GamepadAxes.AxisZm, GamepadAxes.AxisZp)) / parent.divider;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.horizontalCenterOffset: (isKeyboard ? calculateOffsetKeyboard(KeyboardAxes.AxisWm, KeyboardAxes.AxisWp)
                                                          : calculateOffset(GamepadAxes.AxisWm, GamepadAxes.AxisWp)) / parent.divider;
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
                ["AxisX",       GamepadAxes.AxisXm,        GamepadAxes.AxisXp],
                ["AxisY",       GamepadAxes.AxisYm,        GamepadAxes.AxisYp],
                ["AxisZ",       GamepadAxes.AxisZm,        GamepadAxes.AxisZp],
                ["AxisW",       GamepadAxes.AxisWm,        GamepadAxes.AxisWp],
            ]

            UiLabel {
                width: 50;
                horizontalAlignment: Text.AlignHCenter;
                text: calculateOffset(modelData[1], modelData[2]);
                font.family: Style.fontMono;
            }
        }
    }
}
