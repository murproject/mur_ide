import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Layouts 1.12

import input.KeyboardAxes 1.0

Item {
    property string tabTitle: "Keyboard";
    property bool isRebinding: false;
    property bool isKeyboardActive: false;
    property bool restartRequired: false;
    anchors.fill: parent;

    Keys.onPressed: function(event) {
        if (isRebinding) {
            Controllers.keyboard.rebindAxis(event.nativeScanCode);
            isRebinding = false;
            updateKeyboardMode();
        }
    }

    function updateKeyboardMode() {
        isKeyboardActive = Controllers.joystick.getKeyboardMode();
        Controllers.joystick.setKeyboardMode(isKeyboardActive);
    }

    function clearAllAxes() {
        for (var i = 0; i < KeyboardAxes.AxisCount; i++) {
            Controllers.keyboard.clearAxis(i);
        }
    }

    function toDefaultSettings() {
        Controllers.keyboard.toDefaultSettings();
    }

    function save() {
        Controllers.keyboard.saveSettings();
    }

    RowLayout {
        id: column;
        property int buttonWidth: 79;
        spacing: 8;
        anchors.margins: 8;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;

        ColumnLayout {
            Layout.alignment: Qt.AlignTop;
            Layout.fillWidth: true;

            UiLabel {
                text: "Axes and buttons:";
                font.pointSize: Style.headerFontSize;
            }

            AxesRepeater{
                controller: Controllers.keyboard;
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight;

                UiButton {
                    label.text: "Clear all";
                    width: 79;
                    setFocus: false;
                    onClicked: {
                        clearAllAxes();
                    }
                }

                UiButton {
                    label.text: "Default";
                    width: 79;
                    setFocus: false;
                    onClicked: {
                        toDefaultSettings();
                    }
                }
            }
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignCenter;
            Layout.fillWidth: true;

            GamepadWidget {
                Layout.alignment: Qt.AlignHCenter;
                showIndicator: false;
                isKeyboard: true;
                showValues: false;
                opaqueWhenConnected: false;
            }
        }
    }
}
