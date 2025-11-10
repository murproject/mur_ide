import QtQml 2.2
import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: rovPanel;
    anchors.fill: parent;
    color: Style.bgDark;

    ColumnLayout {
        anchors.fill: parent;
        anchors.margins: 16;
        anchors.topMargin: 8;
        spacing: 8;
        Layout.fillWidth: true;
        Layout.fillHeight: false;
        Layout.alignment: Qt.AlignCenter;

        RowLayout {
            Layout.fillWidth: true;

            UiLabel {
                Layout.fillWidth: true;
                textFormat: Text.RichText;
                text: "ProROV settings";
                font.pointSize: Style.headerFontSize;
            }

            UiButton {
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter;
                width: 32;
                icon: icons.fa_times;
                frameless: true;
                onClicked: {
                    vehiclePanel.visible = false;
                }
            }
        }

        Row {
            spacing: 8;

            UiLabel {
                Layout.fillWidth: true;
                textFormat: Text.RichText;
                text: "Servos";
                font.pointSize: Style.headerFontSize;
            }

            UiButton {
                id: servosResetButton;
                label.text: "Reset";
                Layout.alignment: Qt.AlignVCenter;

                onClicked: {
                    control0.reset();
                    control1.reset();
                    control2.reset();
                }
            }

            UiCheckbox {
                id: hideUnset;
                label.text: "Hide unset items"
                Layout.alignment: Qt.AlignVCenter;
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignTop;
            Layout.fillWidth: true;
            spacing: 32;

            GridLayout {
                Layout.alignment: Qt.AlignTop;
                id: servosGrid;
                columns: 1;
                property bool compact: hideUnset.checked;
                rowSpacing: 0;
                columnSpacing: 0;

                UiLabel {
                    text: "FCU 0";
                    visible: !servosGrid.compact;
                }

                RovServoControl {
                    id: control0;
                    device: 0;
                }

                UiLabel {
                    text: "FCU 1";
                    visible: !servosGrid.compact;
                }

                RovServoControl {
                    id: control1;
                    device: 1;
                }

                UiLabel {
                    text: "Onboard";
                    visible: !servosGrid.compact;
                }

                RovServoControl {
                    id: control2;
                    device: 2;
                }

                Item {
                    width: parent.width;
                    height: 1;
                    Layout.fillHeight: true;
                }
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignCenter;

                GamepadWidget {}

                Item {
                    height: 64;
                }

                Row {
                    UiLabel {
                        text: "Motors probing: " + probingPower.value;
                    }
                }

                Slider {
                    id: probingPower;
                    from: -100;
                    to: 100;
                    value: 50;
                    stepSize: 10;
                }

                Repeater {
                    model: 2;

                    Row {
                        property real fcuIndex: modelData;
                        spacing: 4;

                        UiLabel {
                            text: "FCU_" + modelData + ": ";
                        }

                        Repeater {
                            model: 4;

                            UiButton {
                                label.text: modelData;

                                onClicked: {
                                    Controllers.remote.sendRovControl(fcuIndex + 1, modelData, probingPower.value);
                                }

                                onPressing: {
                                    Controllers.remote.sendRovControl(fcuIndex + 1, modelData, probingPower.value);
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true;
                }

            }

        }

        Item {
            Layout.fillHeight: true;
        }

    }
}
