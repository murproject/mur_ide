import QtQml 2.2
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

import mur.GamepadAxes 1.0

ColumnLayout {
    id: root;
    Layout.alignment: Qt.AlignTop;

    property int device: 0;
    property int count: 4;
    property bool compact: parent.compact;

    signal reset();
    Layout.margins: 0;

    Item {
        id: separator;
        height: 1;
        width: parent.width;
    }

    Repeater {
        model: root.count;

        RowLayout {
            id: row;
            property bool active: selector.currentIndex != 0;
            spacing: 4;
            Layout.fillWidth: true;
            Layout.margins: root.compact ? 2 : 0;
            opacity: active ? 1.0 : 0.5;
            visible: !root.compact || active;

            UiLabel {
                font.family: Style.fontMono;
                text: " " + (modelData + 1);
                visible: !root.compact;
            }

            Slider {
                id: sliderServo;
                property int index: modelData;
                from: -100;
                to: 100;
                value: 0;
                stepSize: 10;

                onValueChanged: {
                    Controllers.image.setServoValue(root.device, index, value);
                }
                Connections {
                    target: root;
                    onReset: function() {
                        sliderServo.value = 0;
                    }
                }
                Connections {
                    target: Controllers.image;
                    onServoValueChanged: function(device, servo, value) {
                        if (device === root.device && servo === sliderServo.index) {
                            sliderServo.value = value;
                        }
                    }
                }
            }

            UiLabel {
                Layout.minimumWidth: 32;
                font.family: Style.fontMono;
                text: sliderServo.value;
                horizontalAlignment: Text.AlignRight;
            }

            ComboBox {
                id: selector;
                palette.buttonText: Style.lighterGray;
                model: Controllers.gamepad.getAllFunctionNames();
                currentIndex: Controllers.gamepad.getProrovFunction(device, sliderServo.index);

                onActivated: function(index) {
                    Controllers.gamepad.setProrovFunction(device, sliderServo.index, currentIndex);
                }

                Connections {
                    target: Controllers.gamepad;
                    onProrovFunctionsChanged: {
                        selector.currentIndex = Controllers.gamepad.getProrovFunction(device, sliderServo.index);
                        root.reset();
                    }
                }
            }
        }
    }
}
