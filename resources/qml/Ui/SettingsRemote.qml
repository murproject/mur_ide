import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Layouts 1.12

Item {
    id: settingsRemoteTab;
    property string tabTitle: "Remote mode";
    property bool dangerMode: false;
    property bool restartRequired: true;
    anchors.fill: parent;
    anchors.margins: 8;

    property var controller: Controllers.image;

    function save() {
        Controllers.image.setPipelines(
            watermarkEdit.text
        );

        Controllers.image.saveSpeedLimits();
    }

    function load() {
        var pipes = Controllers.image.getPipelines();
        watermarkEdit.text = pipes[3];
    }

    GridLayout {
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        Layout.fillWidth: true;
        Layout.fillHeight: false;
        columnSpacing: 8;
        columns: 2

        UiLabel { /* placeholder */ }

        UiLabel {
            text: "Movements:";
            font.pointSize: Style.headerFontSize;
        }

        UiLabel {
            Layout.alignment: Qt.AlignTop;
            text: "Speed limits:"
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignTop;

            Repeater {
                id: powerGroup;

                model: 3;

                RowLayout {
                    UiLabel {
                        horizontalAlignment: Text.AlignLeft;
                        Layout.minimumWidth: 70;
                        text: ["Low: ", "Normal: ", "Max: "][index];
                    }

                    Slider {
                        id: powerSlider;
                        Layout.maximumWidth: 200;
                        Layout.fillWidth: true;
                        from: 0;
                        to: 100;
                        stepSize: 5;
                        snapMode: Slider.SnapAlways;
                        value: Controllers.image.getSpeedLimits()[index];

                        onValueChanged: {
                            let min = index == 0 ? 0 :
                                      index == 1 ? Controllers.image.speedLimits[0] :
                                                   Controllers.image.speedLimits[1] ;

                            let max = index == 0 ? Controllers.image.speedLimits[1] :
                                      index == 1 ? Controllers.image.speedLimits[2] :
                                                   100;

                            value = Math.max(Math.min(value, max), min);
                            Controllers.image.setSpeedLimit(index, value);
                        }
                    }

                    UiLabel {
                        Layout.minimumWidth: 30;
                        font.family: Style.fontMono;
                        Layout.alignment: Qt.AlignHCenter;
                        text: powerSlider.value.toFixed(0);
                        horizontalAlignment: Text.AlignRight;
                    }
                }
            }
        }


        UiLabel { /* placeholder */ }

        UiLabel {
            text: "Video streaming:";
            font.pointSize: Style.headerFontSize;
        }

        UiLabel {
            text: "Watermark:"
            Layout.alignment: Qt.AlignTop;
        }

        ColumnLayout {
            RowLayout {
                spacing: 8;

                UiButton {
                    label.text: "Default";
                    onClicked: {
                        watermarkEdit.text = "%device% / %date% / Depth: %depth% M / Temp: %temp% C / Yaw: %yaw%";
                    }
                }

                UiCheckbox {
                    label.text: "Enable watermark";
                    from: Controllers.image;
                    bind: "watermarkOn";
                }
            }

            UiTextEdit {
                id: watermarkEdit;
                text: "%device% / %date% / Depth: %depth% M / Temp: %temp% C / Yaw: %yaw%";
            }

            UiLabel {
                text: "Available variables: date, device, depth, temp, yaw, roll, pitch.";
                wrapMode: "WordWrap";
                font: Style.fontMono;
                Layout.fillWidth: true;
            }
        }

        UiLabel {
            text: ""
        }
    }
}
