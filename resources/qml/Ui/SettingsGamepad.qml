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

    function clearAllAxes() {
        for (var i = 0; i < GamepadAxes.AxisCount; i++) {
            Controllers.joystick.clearAxis(i);
        }
    }

    function toDefaultSettings() {
        Controllers.joystick.toDefaultSettings();
        updateSliders();
    }

    function updateSliders(){
        deadzoneSlider.value = Controllers.joystick.getDeadzone();
        expFactorSlider.value = Controllers.joystick.getExpFactor();
        buttonThresholdSlider.value = Controllers.joystick.getButtonThreshold();
    }

    function updatePresetNames() {
        var currentName = presetComboBox.editText;
        model.clear();
        var presetNames = Controllers.joystick.getPresetNames();
        for (var i = 0; i < presetNames.length; i++) {
            model.append({ text: presetNames[i] });
        }
        presetComboBox.editText = currentName;
    }

    function savePreset() {
        if (presetComboBox.editText !== ""){
            Controllers.joystick.setLastPresetName(presetComboBox.editText);
            Controllers.joystick.saveSettings();
            updatePresetNames();
        }
    }

    function loadPreset() {
        if (presetComboBox.currentText !== ""){
            Controllers.joystick.setLastPresetName(presetComboBox.currentText);
            Controllers.joystick.loadSettings();
            updateSliders();
            calibrationCheckBox.checked = Controllers.joystick.getCalibration();
        }
    }

    function deletePreset(){
        if (presetComboBox.currentText !== ""){
            Controllers.joystick.deletePreset("Preset_" + presetComboBox.currentText);
            updatePresetNames();
            presetComboBox.editText = "";
        }
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

            AxesRepeater{}

            RowLayout {
                Layout.alignment: Qt.AlignRight;
                spacing: 32;

                UiCheckbox{
                    id: showNumbers;
                    label.text: "Show as numbers";
                    checked: true;
                }

                UiButton {
                    label.text: "Clear all";
                    width: 79;
                    onClicked: {
                        clearAllAxes();
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight;
                spacing: 38;

                UiCheckbox{
                    id: calibrationCheckBox;
                    label.text: "Apply calibration";
                    checked: Controllers.joystick.getCalibration();

                    onCheckedChanged: {
                        Controllers.joystick.setCalibration(calibrationCheckBox.checked);
                    }

                    onClicked: {}
                }

                UiButton {
                    label.text: "Default";
                    width: 79;
                    onClicked: {
                        toDefaultSettings();
                    }
                }
            }

            UiLabel {
                text: "Presets:";
                font.pointSize: Style.headerFontSize;
            }

            RowLayout{
                ComboBox {
                    id: presetComboBox;
                    editable: true;
                    Layout.fillWidth: true;
                    Layout.alignment: Qt.AlignVCenter;
                    model: ListModel {
                        id: model
                        Component.onCompleted: {
                            updatePresetNames();
                        }
                    }

                    onActivated: {
                        loadPreset();
                    }

                    Component.onCompleted:{
                        presetComboBox.editText = Controllers.joystick.getLastPresetName();
                    }
                }


                UiButton {
                    label.text: "Save";
                    width: 60;
                    onClicked: {
                        savePreset();
                    }
                }

                UiButton {
                    label.text: "Delete";
                    width: 60;
                    onClicked: {
                        deletePreset();
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
                    id: deadzoneSlider
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
                    id: expFactorSlider;
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

            RowLayout {
                Layout.alignment: Qt.AlignHCenter;
                spacing: 4;

                UiLabel {
                    text: "Threshold:"
                }

                Slider {
                    id: buttonThresholdSlider;
                    Layout.alignment: Qt.AlignVCenter;
                    Layout.fillWidth: true;
                    from: 5;
                    to: 100;
                    value: Controllers.joystick.getButtonThreshold();
                    snapMode: Slider.SnapAlways
                    stepSize: 5;

                    onValueChanged: {
                        Controllers.joystick.setButtonThreshold(value);
                    }
                }

                UiLabel {
                    font.family: Style.fontMono;
                    text: Controllers.joystick.buttonThreshold;
                    horizontalAlignment: Text.AlignRight;
                    Layout.minimumWidth: 25;
                }
            }
        }
    }
}
