import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

import mur.GamepadAxes 1.0

Item {
    property string tabTitle: "Gamepad";
    property bool restartRequired: false;
    anchors.fill: parent;

    function save() {
        Controllers.gamepad.saveSettings();
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
                        text: Controllers.gamepad.getMovementAxisName(modelData) + ":";
                    }

                    UiButton {
                        width: column.buttonWidth;
                        label.font.bold: true;
                        outline: true;
                        label.opacity: label.text == "···" ? 0.25 : 1.0;
                        label.text: Controllers.gamepad.allAxesBindings[modelData];
                        highlight: Controllers.gamepad.rebindingAxis === modelData || Math.abs(Controllers.gamepad.allAxes[modelData]) > 50;
                        onClicked: {
                             Controllers.gamepad.requestRebind(modelData);
                        }
                    }

                    UiButton {
                        highlight: Controllers.gamepad.allAxesInversions[modelData];
                        icon: icons.fa_exchange;
                        toolTip: "Inverse";
                        onClicked: {
                            Controllers.gamepad.setAxisInversion(modelData, !highlight);
                        }
                    }

                    UiButton {
                        icon: icons.fa_times;
                        toolTip: "Clear";
                        onClicked: {
                            Controllers.gamepad.clearAxis(modelData);
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
                    value: Controllers.gamepad.getDeadzone();
                    stepSize: 1;

                    onValueChanged: {
                        Controllers.gamepad.setDeadzone(value);
                    }
                }

                UiLabel {
                    font.family: Style.fontMono;
                    text: Controllers.gamepad.deadzone;
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
                    value: Controllers.gamepad.getExpFactor();
                    snapMode: Slider.SnapAlways
                    stepSize: 0.5;

                    onValueChanged: {
                        value = value.toFixed(1);
                        Controllers.gamepad.setExpFactor(value);
                    }
                }

                UiLabel {
                    font.family: Style.fontMono;
                    text: Controllers.gamepad.expFactor.toFixed(1);
                    horizontalAlignment: Text.AlignRight;
                    Layout.minimumWidth: 22;
                }
            }
        }
    }
}
