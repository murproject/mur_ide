import QtQuick 2.12
import mur 1.0
import QtQuick.Layouts 1.12

import input.KeyboardAxes 1.0

Rectangle {
    property color activeColor: keyboardControl.activeFocus ? Style.greenDark : Style.bgDark;

    gradient: Gradient {
        orientation: Gradient.Vertical;
        GradientStop { position: 0.0; color: Style.bgDark }
        GradientStop { position: 1.0; color: activeColor }
    }

    height: 80;
    Layout.fillWidth: true;
    focus: true;
    clip: true;

    property bool shrink: width < 120;
    property bool active: keyboardControl.activeFocus;

    Behavior on activeColor {
        ColorAnimation {
            duration: Style.animDuration;
            easing.type: Style.animEasing;
        }
    }

    Item {
        id: keyboardControl;
        focus: true;

        property var axisBindings: [];

        Connections {
            target: Controllers.joystick
            function onKeyboardModeChanged() {
                if (Controllers.joystick.getKeyboardMode()) {
                    keyboardControl.forceActiveFocus();
                } else {
                    keyboardControl.focus = false;
                }
            }
        }

        function updateBindings() {
            axisBindings = [
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.AxisXm),    value: -1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.AxisXp),    value:  1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.AxisYm),    value: -1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.AxisYp),    value:  1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.AxisZm),    value: -1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.AxisZp),    value:  1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.AxisWm),    value: -1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.AxisWp),    value:  1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.SpeedFast), value:  1 },
                { key: Controllers.keyboard.getAxisBinding(KeyboardAxes.SpeedSlow), value:  1 }
            ];
        }

        Component.onCompleted: {
            Controllers.keyboard.rebinded.connect(updateBindings);
            updateBindings();
        }

        Keys.onPressed: function(event) {
            let key = event.nativeScanCode;

            for (let binding of axisBindings) {
                if (key === binding.key) {
                    Controllers.keyboard.setAxisValue(binding.key, binding.value);
                }
            }
        }

        Keys.onReleased: function(event) {
            let key = event.nativeScanCode;

            for (let i = 0; i < axisBindings.length; i++) {
                let binding = axisBindings[i];
                if (key === binding.key) {
                    Controllers.keyboard.setAxisValue(binding.key, 0);
                }
            }
        }
    }

    Icon {
        visible: parent.shrink;
        icon: icons.fa_keyboard_o;
        pointSize: 28;
        style: Text.Outline;
        styleColor: Style.outlineColor;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.verticalCenter: parent.verticalCenter;
    }

    UiLabel {
        visible: !parent.shrink;
        opacity: ma.containsMouse && !keyboardControl.activeFocus ? 1.0 : 0.0;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.topMargin: 2;
        anchors.bottomMargin: 2;
        text: keyboardControl.activeFocus ? "" : "Click to control\nwith keyboard";
        horizontalAlignment: Text.AlignHCenter;
        font.bold: true;
        font.pointSize: 16;
        color: Style.lighterGray;
        style: Text.Outline;
        styleColor: Style.outlineColor;

        Behavior on opacity {
            NumberAnimation {
                duration: Style.animDuration;
                easing.type: Style.animEasing;
            }
        }
    }

    GamepadWidget {
        visible: !parent.shrink;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.horizontalCenter: parent.horizontalCenter;
        opacity: (keyboardControl.activeFocus || Controllers.joystick.joystickConnected) && !(!keyboardControl.activeFocus && ma.containsMouse) ? 1.0 : 0.5;
        widgetHeight: 60;
        widgetWidth: widgetHeight;
        showIndicator: false;
        showValues: false;
        opaqueWhenConnected: false;
        isKeyboard: true;

        Behavior on opacity {
            NumberAnimation {
                duration: Style.animDuration;
                easing.type: Style.animEasing;
            }
        }
    }

    Rectangle {
        id: separator;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        height: 1;
        color: Style.outlineColor;
    }

    MouseArea {
        id: ma;
        anchors.fill: parent;
        hoverEnabled: true;
        onClicked: {
            if (!keyboardControl.activeFocus) {
                keyboardControl.forceActiveFocus();
            } else {
                keyboardControl.focus = false;
            }
            Controllers.joystick.setKeyboardMode(keyboardControl.focus);
        }
    }
}
