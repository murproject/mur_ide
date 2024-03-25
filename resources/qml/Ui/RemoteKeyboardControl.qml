import QtQuick 2.12
import mur 1.0
import QtQuick.Layouts 1.12

import mur.GamepadAxes 1.0

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

        property var bindings: {
            "w": [GamepadAxes.AxisY, -1],
            "s": [GamepadAxes.AxisY, +1],
            "a": [GamepadAxes.AxisX, -1],
            "d": [GamepadAxes.AxisX, +1],
            "q": [GamepadAxes.AxisZ, -1],
            "e": [GamepadAxes.AxisZ, +1],
            "z": [GamepadAxes.AxisW, -1],
            "x": [GamepadAxes.AxisW, +1],
        };

        Component.onCompleted: function() {
            var keys = {
                "w": "ц",
                "a": "ф",
                "s": "ы",
                "d": "в",
                "q": "й",
                "e": "у",
                "z": "я",
                "x": "ч",
            };

            for (var key in keys) {
                bindings[keys[key]] = bindings[key];
            }
        }

        Keys.onPressed: {
            if (event.modifiers & Qt.CtrlModifier) {
                Controllers.joystick.setForceAxisValue(GamepadAxes.SpeedFast, 100);
            }

            if (event.modifiers & Qt.ShiftModifier) {
                Controllers.joystick.setForceAxisValue(GamepadAxes.SpeedSlow, 100);
            }

            let action = bindings[event.text];

            if (action !== undefined) {
                let power = action[1] * 40;
                Controllers.joystick.addForceAxisValue(action[0], power);
            }
        }

        Keys.onReleased: {
            let action = bindings[event.text];

            if (!event.modifiers && !event.isAutoRepeat) {
                Controllers.joystick.setForceAxisValue(GamepadAxes.SpeedFast, 0);
                Controllers.joystick.setForceAxisValue(GamepadAxes.SpeedSlow, 0);
            }

            if (action !== undefined && !event.isAutoRepeat) {
                Controllers.joystick.setForceAxisValue(action[0], 0);
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
        }
    }
}
