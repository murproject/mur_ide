import QtQuick 2.9
import QtGraphicalEffects 1.0

Rectangle {
    id: appHeader;
    height: visible ? 32 : 0;
    color: Style.bgDark;
    property var controller: Controllers.menu;

    signal remotePressed;

    Loader {
        id: popupLoader;
        active: false;
        source: "qrc:/qml/Ui/TelemetryPopup.qml";
        anchors.fill: parent;
    }

    Rectangle {
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        height: 1;
        color: Style.bgDarker;
    }

    Row {

        anchors.left: parent.left;
        anchors.top: parent.top;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        anchors.leftMargin: 4;
        spacing: 4;

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;
            frameless: false;
            toolTip: "Run code remote/local";
            width: font.pixelSize * 7;
            label.text: Controllers.scripts.local ? "Local" : "Robot";
            highlight: Controllers.scripts.local;
            icon: Controllers.scripts.local ? icons.fa_desktop : Controllers.network.usv ? icons.fa_ship : icons.fa_rocket;
            iconColor:  !Controllers.scripts.local ? Controllers.network.connected ? Style.green : Style.red : Style.white;
            onClicked: {
                controller.onTargetModeChanged();
                popupLoader.active = false;
            }
        }


        UiButton {
            id: buttonStartScript;
            property bool isRunning: Controllers.scripts.running || Controllers.network.running;

            anchors.verticalCenter: parent.verticalCenter;
            frameless: false;
            icon: isRunning ? icons.fa_refresh : icons.fa_play_circle;
            toolTip: "Start programm [F5]";
            enabled: Controllers.scripts.local ? !Controllers.scripts.running : Controllers.network.connected ? !Controllers.network.running : false;
            shortcut.sequence: "F5";
            iconRotation: isRunning ? iconRotation : 0;

            onClicked: {
                controller.onFileSave();
                controller.onCodeRun();
            }

            SequentialAnimation on iconRotation {
                id: rotation;
                running: Controllers.scripts.running || Controllers.network.running;
                loops: Animation.Infinite;
                PropertyAnimation { from: 0; to: 360; duration: 3000; }
            }
        }

        UiButton {
            id: buttonStopScript;
            anchors.verticalCenter: parent.verticalCenter;

            frameless: false;
            icon: icons.fa_stop_circle;
            toolTip: "Stop programm [F6]";
            enabled: Controllers.scripts.local ? Controllers.scripts.running : Controllers.network.connected ? Controllers.network.running : false;            
            shortcut.sequence: "F6";
            onClicked: {
                controller.onCodeStop();
            }
        }

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;

            frameless: false;
            icon: icons.fa_save;
            toolTip: "Save source file";
            onClicked: {
                controller.onFileSave();
            }
        }

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;
            frameless: false;
            icon: icons.fa_folder_open;
            toolTip: "Open source file";
            onClicked: {
                controller.onFileOpen();
            }
        }

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;
            frameless: false;
            icon: icons.fa_file;
            toolTip: "New source file";
            onClicked: {
                controller.onFileNew();
            }
        }

        UiButton {
            id: buttonStartRemote;
            anchors.verticalCenter: parent.verticalCenter;

            frameless: false;
            icon: icons.fa_gamepad;
            toolTip: "Start remote mode";
            // TODO:
            enabled: !Controllers.scripts.local && Controllers.network.connected && !Controllers.network.running;
            highlight: Controllers.network.remote && Controllers.network.connected;
            shortcut.sequence: "F7";

            onClicked: {
               controller.onRunRemote();
            }
        }
    }

    Row {
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.rightMargin: 4;
        spacing: 4;

        UiButton {
            id: vehicle_button;
            anchors.verticalCenter: parent.verticalCenter;
            frameless: false;
            toolTip: "Vehicle info";
            enabled: true;
            visible: Controllers.network.connected && !Controllers.scripts.local;
            label.text: Controllers.network.vehicle_name;
            label.textFormat: Text.RichText;
            iconColor: Style.batteryColor;
            iconOpacity: 1.0;

            Icon {
                visible: !Controllers.network.rov;

                parent: vehicle_button.after_item;
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;
                verticalAlignment: Text.AlignVCenter;
                font.pixelSize: 14;
                icon: " " + icons.fa_bolt;
                color: Style.yellow;
                opacity: 0;
                style: Text.Outline;
                styleColor:  "#4A4709";

                SequentialAnimation on opacity {
                    running: Controllers.network.is_charging;
                    alwaysRunToEnd: true;
                    loops: Animation.Infinite;
                    PropertyAnimation { from: 0.0; to: 1.0; duration: 1000; easing.type: Easing.OutQuart; }
                    PropertyAnimation { from: 1.0; to: 0.0; duration: 1000; easing.type: Easing.OutQuart; }
                }
            }

            icon: {
                if (Controllers.network.rov) {
                    icon: "";
                } else if (Controllers.network.battery >= 90) {
                    icon: icons.fa_battery_4;
                } else if (Controllers.network.battery >= 70) {
                    icon: icons.fa_battery_3;
                } else if (Controllers.network.battery >= 50) {
                    icon: icons.fa_battery_2;
                } else if (Controllers.network.battery >= 20) {
                    icon: icons.fa_battery_1;
                } else {
                    icon: icons.fa_battery_0;
                }
            }
        }

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;
            frameless: false;
            toolTip: "Open simulator";
            enabled: !Controllers.simulator.running;
            icon: icons.fa_bug;
            onClicked: {
               controller.onRunSimulator();
            }
        }

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;
            frameless: false;
            toolTip: "Telemetry";
            shortcut.sequence: "F1";

            icon: icons.fa_info_circle;
            highlight: popupLoader.active;
            enabled: !Controllers.scripts.local && Controllers.network.connected;
            onClicked: {
                if(!popupLoader.active) {
                    popupLoader.active = true;
                }
                else {
                    popupLoader.active = false;
                }
            }
        }
    }

}
