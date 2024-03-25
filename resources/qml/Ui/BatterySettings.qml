import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Layouts 1.12

Popup {
    id: batteryPopup;
    x: (parent.width - width) / 2
    y: menuBar.y
    width: parent.width / 1;
    height: parent.height + menuBar.height;
    closePolicy: Popup.CloseOnEscape;
    modal: true;
    visible: true;
    parent: fullWindow;

    background: Rectangle {
        id: back;
        anchors.fill: parent;
        color: Style.bgDark;
        border.width: 1;
        border.color: Style.bgDarker;
    }

    Overlay.modal: Rectangle {
        color: Style.overlayBack;
        Behavior on opacity {
           NumberAnimation {
               duration: Style.animFastest
           }
       }
    }

    ColumnLayout {
        anchors.top: parent.top;
        width: Math.min(parent.width, 900);
        anchors.horizontalCenter: parent.horizontalCenter;

        Row {
            Layout.alignment: Qt.AlignHCenter;

            UiLabel {
                anchors.verticalCenter: parent.verticalCenter;
                font.pointSize: Style.headerFontSize;
                text: "Battery settings";
            }
        }

        Rectangle {
            Layout.fillWidth: true;
            width: parent.width;
            height: 1;
            color: Style.bgBlue;
        }

        GridLayout{
            id: grid;
            columns: 2;
            rowSpacing: 10;
            columnSpacing: 15;
            Layout.leftMargin: 35;
            Layout.topMargin: 25;

            UiLabel{
                text: "Cycle count\n" + "Full charge capacity\n" +
                      "Max error\n" + "Remaining capacity\n" +
                      "Reset count\n" + "Update status";
            }
            UiLabel{
                text: Controllers.network.fg_cycle_count + "\n" +
                      Controllers.network.fg_full_charge_capacity + "\n" +
                      Controllers.network.fg_max_error + "\n" +
                      Controllers.network.fg_remaining_capacity + "\n" +
                      Controllers.network.fg_reset_count + "\n" +
                      Controllers.network.fg_update_status;
            }
            UiLabel{
                text: "Temperature\n" + "Amperage\n" + "Voltage";
            }
            UiLabel{
                text: Controllers.network.fg_temp.toFixed(2) + " °C\n" +
                      Controllers.network.amperage.toFixed(2) + " A\n" +
                      Controllers.network.voltage.toFixed(2) + " V";
            }
            UiLabel{
                text: "Flags:\n" + "FC\n" + "QEN\n" +
                      "RUP_DIS\n" + "VOK\n" + "OCVTAKEN";
            }

            UiLabel{
                text: "\n" + Controllers.network.fg_flag_fc + "\n" +
                      Controllers.network.fg_flag_qen + "\n" +
                      Controllers.network.fg_flag_rup_dis + "\n" +
                      Controllers.network.fg_flag_vok_flag + "\n" +
                      Controllers.network.fg_flag_ocvtaken;
            }
            UiButton {
                label.text: "RESET";
                width: 120;
                enabled: resetConfirm.checked;
                onClicked: {
                    Controllers.network.batteryButton("reset");
                }
            }
            UiCheckbox{
                id: resetConfirm;
                label.text: "Are you sure?";
                checked: false;
            }
            UiButton {
                label.text: "IT_ENABLE";
                width: 120;
                enabled: itEnableConfirm.checked;
                onClicked: {
                    Controllers.network.batteryButton("it_enable");
                }
            }
            UiCheckbox{
                id: itEnableConfirm;
                label.text: "Are you sure?";
                checked: false;
            }
        }
    }

    Row {
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;

        UiButton {
            id: batteryOkButton;
            anchors.verticalCenter: parent.verticalCenter;
            label.text: "OK";
            label.font.bold: true;
            onClicked: {
                batteryPopup.close();
            }
        }

    }

}
