import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4

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
                label.text: Controllers.image.gamepad.axisXName;
                highlight: Controllers.image.gamepad.rebindX;
                onClicked: {
                    Controllers.image.gamepad.rebindAxisX();
                }
            }
            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: Controllers.image.gamepad.inverseX;
                label.text: Controllers.image.gamepad.inverseX ? "   Inversed  " : "Not inversed" ;
                onClicked: {
                    Controllers.image.gamepad.inverseX = !Controllers.image.gamepad.inverseX;
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
                highlight: Controllers.image.gamepad.rebindY;
                label.text: Controllers.image.gamepad.axisYName;
                onClicked: {
                    Controllers.image.gamepad.rebindAxisY();
                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: Controllers.image.gamepad.inverseY;
                label.text: Controllers.image.gamepad.inverseY ? "   Inversed  " : "Not inversed" ;
                onClicked: {
                    Controllers.image.gamepad.inverseY = !Controllers.image.gamepad.inverseY;
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
                highlight: Controllers.image.gamepad.rebindZ;
                label.text: Controllers.image.gamepad.axisZName;
                onClicked: {
                    Controllers.image.gamepad.rebindAxisZ();
                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                frameless: false;
                highlight: Controllers.image.gamepad.inverseZ;
                label.text: Controllers.image.gamepad.inverseZ ? "   Inversed  " : "Not inversed" ;
                onClicked: {
                    Controllers.image.gamepad.inverseZ = !Controllers.image.gamepad.inverseZ;
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
