import QtQuick 2.9


Rectangle {
    id: appHeader;
    height: visible ? 32 : 0;
    color: "#21252B";
    property var controller: Controllers.menu;


    signal remotePressed;

    Loader {
        id: popupLoader;
        active: false;
        source: "qrc:/qml/Ui/TelimetryPopup.qml";
        anchors.fill: parent
    }

    Rectangle {
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        height: 1;
        color: "#181A1F";
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
            label.text: Controllers.scripts.local ? "Local" : "Robot";
            highlight: Controllers.scripts.local;
            icon: Controllers.scripts.local ? icons.fa_desktop : Controllers.network.usv ? icons.fa_ship : icons.fa_rocket;
            iconColor:  !Controllers.scripts.local ? Controllers.network.connected ? "#148F77" : "#E74C3C" : "#fff";
            onClicked: {
                controller.onTargetModeChanged();
            }
        }


        UiButton {
            anchors.verticalCenter: parent.verticalCenter;

            frameless: false;
            icon: icons.fa_play_circle;
            toolTip: "Start programm";
            enabled: Controllers.scripts.local ? !Controllers.scripts.running : Controllers.network.connected ? !Controllers.network.running : false;

            onClicked: {
                controller.onFileSave();
                controller.onCodeRun();
            }
        }

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;

            frameless: false;
            icon: icons.fa_stop_circle;
            toolTip: "Stop programm";
            enabled: Controllers.scripts.local ? Controllers.scripts.running : Controllers.network.connected ? Controllers.network.running : false;
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
            anchors.verticalCenter: parent.verticalCenter;

            frameless: false;
            icon: icons.fa_gamepad;
            toolTip: "Start remote mode";
            enabled: !Controllers.scripts.local && Controllers.network.connected && !Controllers.network.running;
            highlight: Controllers.network.remote && Controllers.network.connected;

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
            toolTip: "Telimetry";

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
