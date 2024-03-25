import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Layouts 1.12

import mur.GamepadAxes 1.0

Item {
    property string tabTitle: "Gamepad";
    property bool restartRequired: false;
    anchors.fill: parent;

    function save() {
        Controllers.joystick.saveSettings();
    }

    RowLayout {
        id: column;
        property int buttonWidth: 60;
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

            Repeater {
                model: [
                    GamepadAxes.AxisX,
                    GamepadAxes.AxisY,
                    GamepadAxes.AxisZ,
                    GamepadAxes.AxisW,
                ];

                RowLayout {
                    visible: modelData != GamepadAxes.AxisW;

                    UiLabel {
                        Layout.fillWidth: true;
                        text: Controllers.joystick.getMovementAxisName(modelData) + ":";
                    }

                    UiButton {
                        width: column.buttonWidth;
                        label.font.bold: true;
                        outline: true;
                        label.opacity: label.text == "···" ? 0.25 : 1.0;
                        label.text: Controllers.joystick.allAxesBindings[modelData];
                        highlight: Controllers.joystick.rebindingAxis === modelData || Math.abs(Controllers.joystick.allAxes[modelData]) > 50;
                        onClicked: {
                             Controllers.joystick.requestRebind(modelData);
                        }
                    }

                    UiButton {
                        highlight: Controllers.joystick.allAxesInversions[modelData];
                        icon: icons.fa_exchange;
                        toolTip: "Inverse";
                        onClicked: {
                            Controllers.joystick.setAxisInversion(modelData, !highlight);
                        }
                    }

                    UiButton {
                        icon: icons.fa_times;
                        toolTip: "Clear";
                        onClicked: {
                            Controllers.joystick.clearAxis(modelData);
                        }
                    }
                }
            }
            Repeater {
                model: [
                    ["Speed",       GamepadAxes.SpeedSlow,        GamepadAxes.SpeedFast],
                ];

                RowLayout {
                   visible: index == 0 || showAll.checked;

                    UiLabel {
                        Layout.fillWidth: true;
                        text: modelData[0] + ":\t";
                    }

                    UiButton {
                        label.font.bold: true;
                        outline: true;
                        label.opacity: label.text == "···" ? 0.25 : 1.0;
                        width: column.buttonWidth;
                        label.text: Controllers.joystick.allAxesBindings[modelData[1]];
                        highlight: Controllers.joystick.rebindingAxis === modelData[1] || Controllers.joystick.allAxes[modelData[1]] > 50;
                        onClicked: {
                             Controllers.joystick.requestRebind(modelData[1]);
                        }
                    }

                    UiButton {
                        frameless: true;
                        enabled: false;
                        width: 25;
                        icon: Controllers.joystick.allAxes[modelData[2]] > 50 ? icons.fa_chevron_up   :
                              Controllers.joystick.allAxes[modelData[1]] > 50 ? icons.fa_chevron_down :
                                                                               "·";
                    }


                    UiButton {
                        label.font.bold: true;
                        outline: true;
                        label.opacity: label.text == "···" ? 0.25 : 1.0;
                        width: column.buttonWidth;
                        label.text: Controllers.joystick.allAxesBindings[modelData[2]];
                        highlight: Controllers.joystick.rebindingAxis === modelData[2] || Controllers.joystick.allAxes[modelData[2]] > 50;
                        onClicked: {
                             Controllers.joystick.requestRebind(modelData[2]);
                        }
                    }

                    UiButton {
                        highlight: Controllers.joystick.allAxesInversions[modelData[1]];
                        icon: icons.fa_exchange;
                        toolTip: "Inverse";
                        onClicked: {
                            Controllers.joystick.swapAxes(modelData[1], modelData[2]);
                        }
                    }

                    UiButton {
                        icon: icons.fa_times;
                        toolTip: "Clear";
                        onClicked: {
                            Controllers.joystick.clearAxis(modelData[1]);
                            Controllers.joystick.clearAxis(modelData[2]);
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter;
            Layout.fillWidth: true;

            GamepadWidget {
                Layout.bottomMargin: 56;
                Layout.alignment: Qt.AlignHCenter;
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter;
                spacing: 4;

                UiLabel {
                    text: "Deadzone:"
                }

                Slider {
                    Layout.alignment: Qt.AlignVCenter;
                    Layout.fillWidth: true;
                    from: 0;
                    to: 90;
                    value: Controllers.joystick.getDeadzone();
                    stepSize: 1;

                    onValueChanged: {
                        Controllers.joystick.setDeadzone(value);
                    }
                }

                UiLabel {
                    font.family: Style.fontMono;
                    text: Controllers.joystick.deadzone;
                    horizontalAlignment: Text.AlignRight;
                    Layout.minimumWidth: 22;
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter;
                spacing: 4;

                UiLabel {
                    text: "Exp factor:"
                }

                Slider {
                    Layout.alignment: Qt.AlignVCenter;
                    Layout.fillWidth: true;
                    from: 1.0;
                    to: 5.0;
                    value: Controllers.joystick.getExpFactor();
                    snapMode: Slider.SnapAlways
                    stepSize: 0.5;

                    onValueChanged: {
                        value = value.toFixed(1);
                        Controllers.joystick.setExpFactor(value);
                    }
                }

                UiLabel {
                    font.family: Style.fontMono;
                    text: Controllers.joystick.expFactor.toFixed(1);
                    horizontalAlignment: Text.AlignRight;
                    Layout.minimumWidth: 22;
                }
            }
        }
    }
}
