import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Layouts 1.12

import mur.GamepadAxes 1.0
import input.KeyboardAxes 1.0

Repeater {
    property var controller: Controllers.joystick;
    property bool isKeyboard: controller === Controllers.keyboard ? true : false;
    property var axes : isKeyboard ? KeyboardAxes : GamepadAxes;
    property real buttonThreshold: isKeyboard ? 0 : controller.buttonThreshold;

    model: [
        ["Axis X (yaw)",           axes.AxisXm,        axes.AxisXp],
        ["Axis Y (forward)",       axes.AxisYm,        axes.AxisYp],
        ["Axis Z (depth)",         axes.AxisZm,        axes.AxisZp],
        ["Speed",                  axes.SpeedFast,     axes.SpeedSlow],
    ];

    RowLayout {
        UiLabel {
            Layout.fillWidth: true;
            text: modelData[0] + ":";
        }

        UiButton {
            label.font.bold: true;
            outline: true;
            label.opacity: label.text === "···" ? 0.25 : 1.0;
            width: column.buttonWidth;
            label.text: isKeyboard ? controller.allAxesBindings[modelData[1]] !== "None" ? controller.allAxesBindings[modelData[1]] :  "···"
                                   : showNumbers.checked ? controller.allAxesBindings[modelData[1]].axis_index : controller.allAxesBindings[modelData[1]].axis_name;
            highlight: controller.rebindingAxis === modelData[1] || Math.abs(controller.allAxes[modelData[1]]) > buttonThreshold;
            onClicked: {
                if (isKeyboard) {
                    isRebinding = true;
                    controller.requestRebind(modelData[1]);
                } else {
                    if (controller.rebindingAxis === modelData[1]) {
                        controller.requestRebind(0);
                    } else {
                        controller.requestRebind(modelData[1]);
                    }
                }
            }
        }

        UiButton {
            frameless: true;
            enabled: false;
            width: 25;
            icon: Math.abs(controller.allAxes[modelData[2]]) > buttonThreshold ? icons.fa_chevron_down :
                  Math.abs(controller.allAxes[modelData[1]]) > buttonThreshold ? icons.fa_chevron_up   :
                                                                   "·";
        }


        UiButton {
            label.font.bold: true;
            outline: true;
            label.opacity: label.text === "···" ? 0.25 : 1.0;
            width: column.buttonWidth;
            label.text: isKeyboard ? controller.allAxesBindings[modelData[2]] !== "None" ? controller.allAxesBindings[modelData[2]] :  "···"
                                   : showNumbers.checked ? controller.allAxesBindings[modelData[2]].axis_index : controller.allAxesBindings[modelData[2]].axis_name;
            highlight: controller.rebindingAxis === modelData[2] || Math.abs(controller.allAxes[modelData[2]]) > buttonThreshold;
            onClicked: {
                if (isKeyboard) {
                    isRebinding = true;
                    controller.requestRebind(modelData[2]);
                } else {
                    if (controller.rebindingAxis === modelData[2]) {
                        controller.requestRebind(0);
                    } else {
                        controller.requestRebind(modelData[2]);
                    }
                }
            }
        }

        UiButton {
            icon: icons.fa_exchange;
            toolTip: "Inverse";
            setFocus: !isKeyboard;
            onClicked: {
                controller.swapAxes(modelData[1], modelData[2]);
            }
        }

        UiButton {
            icon: icons.fa_times;
            toolTip: "Clear";
            setFocus: !isKeyboard;
            onClicked: {
                controller.clearAxis(modelData[1]);
                controller.clearAxis(modelData[2]);
            }
        }
    }
}
