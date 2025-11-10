import QtQuick 2.12
import mur 1.0
import QtQuick.Layouts 1.12

import mur.GamepadAxes 1.0
import input.KeyboardAxes 1.0

Rectangle {
    id: remoteView;
    property var controller: Controllers.remote;
    property string tabTitle: controller.recordingVideo ? "Remote (recording)" : "Remote";
    height: parent.height;
    width: parent.width;
    color: Style.bgBlue;
    property bool shrink: Controllers.network.rov ? width < 450 : width < 300;

    function calculateValue(index1, index2) {
            var value1 = Controllers.joystick.allAxes[index1];
            var value2 = Controllers.joystick.allAxes[index2];
            return (value1 > value2) ? -value1 : value2;
        }

    function calculateValueKeyboard(index1, index2) {
        var value1 = Controllers.keyboard.allAxes[index1] * 100;
        var value2 = Controllers.keyboard.allAxes[index2] * 100;
        return (Math.abs(value1) > Math.abs(value2)) ? value1 : value2;
    }

    Rectangle {
        id: remoteHeader;
        height: 22;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        color: Style.bgDark;

        Row {
            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.bottom: parent.bottom;
            anchors.leftMargin: 8;
            anchors.rightMargin: 8;
            spacing: 4;

            UiButton {
                frameless: true;
                anchors.verticalCenter: parent.verticalCenter;
                icon: icons.fa_angle_down;
                rotation: keyboardControl.visible ? 180 : 0;
                highlight: keyboardControl.visible;

                onClicked: {
                    keyboardControl.visible = !keyboardControl.visible;
                }
            }

            Icon {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.verticalCenterOffset: -1;
                icon: keyboardControl.active ? icons.fa_keyboard_o : icons.fa_gamepad;
                color: keyboardControl.active ? Style.white : Controllers.joystick.joystickConnected ? Style.green : Style.red;
            }

            UiLabel {
                visible: remoteView.width > 200;
                anchors.verticalCenter: parent.verticalCenter;
                text: remoteView.shrink ? "Axes:" : "Gamepad axes:";
                font.pointSize: 10;
            }
        }

        Row {
            spacing: 4;
            width: parent.width < 200 ? parent.width - 40 : Math.min(parent.width / 2, 200);
            anchors.verticalCenter: parent.verticalCenter;
            anchors.right: parent.right;
            anchors.rightMargin: 4;
            opacity: parent.width > 140 ? 1.0 : 0.0;

            UiLabel {
                width: parent.width / 4;
                font.family: Style.fontMono;
                anchors.verticalCenter: parent.verticalCenter;
                text: Controllers.joystick.keyboardMode ? calculateValueKeyboard(KeyboardAxes.AxisXm,  KeyboardAxes.AxisXp)
                                                             : calculateValue(GamepadAxes.AxisXm, GamepadAxes.AxisXp);
                horizontalAlignment: Text.AlignRight;
                font.pointSize: 10;
            }

            UiLabel {
                width: parent.width / 4;
                font.family: Style.fontMono;
                anchors.verticalCenter: parent.verticalCenter;
                text: Controllers.joystick.keyboardMode ? calculateValueKeyboard(KeyboardAxes.AxisYm,  KeyboardAxes.AxisYp) :
                                                               calculateValue(GamepadAxes.AxisYm, GamepadAxes.AxisYp);
                horizontalAlignment: Text.AlignRight;
                font.pointSize: 10;
            }

            UiLabel {
                width: parent.width / 4;
                font.family: Style.fontMono;
                anchors.verticalCenter: parent.verticalCenter;
                text: Controllers.joystick.keyboardMode ? calculateValueKeyboard(KeyboardAxes.AxisZm,  KeyboardAxes.AxisZp) :
                                                               calculateValue(GamepadAxes.AxisZm, GamepadAxes.AxisZp);
                horizontalAlignment: Text.AlignRight;
                font.pointSize: 10;
            }
        }
    }

    Rectangle {
        id: separator;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: remoteHeader.bottom;
        height: 1;
        color: Style.bgDarker;
        visible: !keyboardControl.visible;
    }

    ColumnLayout {
        id: remoteMain;
        anchors.left: parent.left;
        anchors.top: separator.bottom;
        anchors.right: parent.right;
        anchors.bottom: remoteFooter.top;
        spacing: 0;

        RemoteKeyboardControl {
            id: keyboardControl;
            visible: false;
        }

        GridLayout {
            id: imageView;
            property bool isVertical: width / height < 4 / 3;
            columns: isVertical ? 1 : 2;
            rowSpacing: 0;
            columnSpacing: 0;
            Layout.alignment: Qt.AlignVCenter;

            RemoteCamera {
                Layout.fillWidth: true;
                Layout.fillHeight: true;
                id: front;
                image: controller.front;
                subtitle: "Camera 0";
                function saveImage() {
                    controller.saveImageFront();
                }
            }

            RemoteCamera {
                id: bottom;
                Layout.fillWidth: true;
                Layout.fillHeight: true;
                visible: !Controllers.network.usv && !Controllers.network.rov;
                image: controller.bottom;
                subtitle: "Camera 1";
                function saveImage() {
                    controller.saveImageBottom();
                }
            }
        }
    }

    Rectangle {
        id: remoteFooter;
        height: 32;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        color: Style.bgDark;
        clip: true;

        property bool autoButtonsVisible: Controllers.network.rov ? remoteView.width > 240 : remoteView.width > 160;

        Row {
            id: remoteToolbar;
            property bool collapsed: false;
            anchors.top: fullWindow.visible ? undefined : parent.top;
            anchors.bottom: parent.bottom;
            anchors.horizontalCenter: collapsed ? parent.left : parent.horizontalCenter;
            leftPadding: collapsed ? 34 : 0;
            anchors.rightMargin: 4;
            height: 22;
            spacing: 4;

            parent: fullWindow.visible ? fullWindow : remoteFooter;
            opacity: fullWindow.visible ? 0.6 : 1.0;

            UiButton {
                id: buttonCapture;
                anchors.verticalCenter: parent.verticalCenter;
                label.text: remoteView.width < 420 ? "" : "Capture";
                toolTip: "Capture both images [F8]";
                shortcut.sequence: "F8";
                icon: icons.fa_camera;
                visible: !remoteToolbar.collapsed;
                setFocus: false;

                onClicked: {
                    if (!front.haveImage && !bottom.haveImage) {
                        notifications.notifyEmptyImage();
                    } else {
                        front.saveImage();
                        bottom.saveImage();
                        notifications.notifyImageSaved();
                    }
                }
            }

            UiButton {
                id: buttonRecord;
                anchors.verticalCenter: parent.verticalCenter;
                label.text: remoteView.width < 420 ? "" : "Record";
                toolTip: "Record video from cameras [F9]";
                shortcut.sequence: "F9";
                icon: controller.recordingVideo ? icons.fa_circle : icons.fa_video_camera;
                iconColor: controller.recordingVideo ? Style.red : Style.lightGray;
                visible: !remoteToolbar.collapsed;
                setFocus: false;

                onClicked: {
                    if (!controller.recordingVideo && !front.haveImage && !bottom.haveImage) {
                        notifications.notifyEmptyImage();
                    } else {
                        controller.recordingVideo = !controller.recordingVideo;
                    }
                }

                SequentialAnimation on iconOpacity {
                    running: controller.recordingVideo;
                    alwaysRunToEnd: true;
                    loops: Animation.Infinite;
                    PropertyAnimation { from: 1.0; to: 0.0; duration: 150; }
                    PropertyAnimation { from: 0.0; to: 1.0; duration: 150; }
                    PropertyAnimation { to: 1.0; duration: 700; }
                }
            }

            UiButton {
                visible: fullWindow.visible && !remoteToolbar.collapsed && remoteFooter.autoButtonsVisible;
                anchors.verticalCenter: parent.verticalCenter;
                toolTip: "Toggle compass";
                label.text: "Compass";
                icon : icons.fa_compass;
                highlight: compass.visible;
                setFocus: false;
                onClicked: {
                    compass.visible = !compass.visible;
                }
            }

            UiButton {
                visible: fullWindow.visible && !remoteToolbar.collapsed && remoteFooter.autoButtonsVisible;
                anchors.verticalCenter: parent.verticalCenter;
                toolTip: "Toggle altimeter";
                label.text: "Altimeter";
                icon : icons.fa_arrows_v;
                highlight: altimeter.visible;
                setFocus: false;
                onClicked: {
                    altimeter.visible = !altimeter.visible;
                }
            }


            UiButton {
                id: controlModeButton;
                property bool switchMode: false;
                visible: fullWindow.visible && !remoteToolbar.collapsed && remoteFooter.autoButtonsVisible;
                anchors.verticalCenter: parent.verticalCenter;
                toolTip: "Switch controls";
                label.text: "Control mode";
                icon: switchMode ? icons.fa_keyboard_o : icons.fa_gamepad;
                highlight: switchMode;


                Connections {
                    target: keyboardControl
                    function onActiveChanged() {
                        if (keyboardControl.active) {
                            controlModeButton.switchMode = true;
                        } else {
                            controlModeButton.switchMode = false;
                        }
                    }
                }

                onClicked: {
                    Controllers.joystick.setKeyboardMode(!Controllers.joystick.getKeyboardMode());
                }
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter;
                width: 1;
                height: 18;
                color: Style.bgDarker;
                visible: !remoteToolbar.collapsed && remoteFooter.autoButtonsVisible;
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                highlight: controller.autoYawAltmode;
                label.text: remoteView.width < 420 ? "A" : "Alt Yaw";
                toolTip: "Yaw regulator Alt mode";
                visible: !remoteToolbar.collapsed && Controllers.network.rov && remoteFooter.autoButtonsVisible;
                setFocus: false;
                onClicked: {
                    controller.autoYawAltmode = !controller.autoYawAltmode;
                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                highlight: controller.autoYaw;
                label.text: remoteView.shrink ? "Y" : Controllers.network.rov ? "Yaw" : "Auto Yaw";
                toolTip: "Auto Yaw regulator";
                icon: Controllers.network.rov ? "" : icons.fa_compass;
                visible: !remoteToolbar.collapsed && remoteFooter.autoButtonsVisible;
                setFocus: false;
                onClicked: {
                    controller.autoYaw = !controller.autoYaw;
                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                highlight: controller.autoPitch;
                label.text: remoteView.shrink ? "P" : "Pitch";
                toolTip: "Auto Pitch regulator";
                visible: !remoteToolbar.collapsed && Controllers.network.rov && remoteFooter.autoButtonsVisible;
                setFocus: false;
                onClicked: {
                    controller.autoPitch = !controller.autoPitch;
                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                highlight: controller.autoRoll;
                label.text: remoteView.shrink ? "R" : "Roll";
                toolTip: "Auto Roll regulator";
                visible: !remoteToolbar.collapsed && Controllers.network.rov && remoteFooter.autoButtonsVisible;
                setFocus: false;
                onClicked: {
                    controller.autoRoll = !controller.autoRoll;
                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                highlight: controller.autoDepth;
                label.text: remoteView.shrink ? "D" : Controllers.network.rov ? "Depth" : "Auto Depth";
                toolTip: "Auto Depth regulator";
                icon: Controllers.network.rov ? "" : icons.fa_arrows_v;
                visible: !remoteToolbar.collapsed && remoteFooter.autoButtonsVisible;
                setFocus: false;
                onClicked: {
                    controller.autoDepth = !controller.autoDepth;
                }
            }

            UiButton {
                id: regulatorsToggle;
                anchors.verticalCenter: parent.verticalCenter;
                property bool allOff: !(controller.autoYaw || controller.autoDepth || controller.autoRoll || controller.autoPitch);
                highlight: !allOff;
                icon: icons.fa_toggle_on;
                rotation: allOff ? 180 : 0;
                toolTip: "Enable/disable all regulators";
                visible: !remoteToolbar.collapsed && remoteFooter.autoButtonsVisible;
                frameless: true;
                setFocus: false;
                onClicked: {
                    let state = allOff;
                    controller.autoYaw = state;
                    controller.autoDepth = state;
                    controller.autoRoll = state;
                    controller.autoPitch = state;
                }
            }

            Rectangle {
                visible: fullWindow.visible && !remoteToolbar.collapsed;
                anchors.verticalCenter: parent.verticalCenter;
                width: 1;
                height: 18;
                color: Style.bgDarker;
            }

            UiButton {
                visible: fullWindow.visible;
                anchors.verticalCenter: parent.verticalCenter;
                toolTip: "Toggle panel";
                icon : remoteToolbar.collapsed ? icons.fa_arrow_right : icons.fa_arrow_left;
                setFocus: false;
                onClicked: {
                    remoteToolbar.collapsed = !remoteToolbar.collapsed;
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
