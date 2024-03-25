import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2

Popup {
    id: gamepadPopup;
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    closePolicy: Popup.CloseOnEscape;
    modal: true;
    visible: true;
    parent: ApplicationWindow.overlay;
    width: 330;
    height: 260;

    background: Rectangle {
        anchors.fill: parent;
        color: "#21252B";
        border.width: 1;
        border.color: "#181A1F";
    }

    Column {
        spacing: 4

        Row {
            spacing: 4;
            UiLabel {
                text: "Axis for X movement:"
            }
            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                label.text: Controllers.image.Gamepad.axisXName;
                highlight: Controllers.image.Gamepad.rebindX;
                onClicked: {
                    Controllers.image.Gamepad.rebindAxisX();
                }
            }
            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: Controllers.image.Gamepad.inverseX;
                label.text: Controllers.image.Gamepad.inverseX ? "   Inversed  " : "Not inversed" ;
                onClicked: {
                    Controllers.image.Gamepad.inverseX = !Controllers.image.Gamepad.inverseX;
                }
            }
        }

        Row {
            spacing: 4;
            UiLabel {
                text: "Axis for Y movement:"
            }
            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: Controllers.image.Gamepad.rebindY;
                label.text: Controllers.image.Gamepad.axisYName;
                onClicked: {
                    Controllers.image.Gamepad.rebindAxisY();
                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: Controllers.image.Gamepad.inverseY;
                label.text: Controllers.image.Gamepad.inverseY ? "   Inversed  " : "Not inversed" ;
                onClicked: {
                    Controllers.image.Gamepad.inverseY = !Controllers.image.Gamepad.inverseY;
                }
            }
        }

        Row {
            spacing: 4;
            UiLabel {
                text: "Axis for Z movement:"
            }
            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: Controllers.image.Gamepad.rebindZ;
                label.text: Controllers.image.Gamepad.axisZName;
                onClicked: {
                    Controllers.image.Gamepad.rebindAxisZ();
                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: Controllers.image.Gamepad.inverseZ;
                label.text: Controllers.image.Gamepad.inverseZ ? "   Inversed  " : "Not inversed" ;
                onClicked: {
                    Controllers.image.Gamepad.inverseZ = !Controllers.image.Gamepad.inverseZ;
                }
            }
        }
    }

    Rectangle {
        id: gamepadFooter;
        height: 22;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        color: "#21252B";

        Row {
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.left: parent.left;
            anchors.leftMargin: 8;
            spacing: 4;

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: true;
                label.text: "Okay";
                onClicked: {
                    gamepadPopup.close();
                }
            }
        }
    }
}
