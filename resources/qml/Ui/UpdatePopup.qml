import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4

Popup {
    id: updatePopup;
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside;
    modal: true;
    visible: true;
    parent: ApplicationWindow.overlay;
    width: 350;
    height: 120;
    property var controller: Controllers.updates;

    background: Rectangle {
        anchors.fill: parent;
        color: "#21252B";
        border.width: 0;
        border.color: "#181A1F";
    }

    Rectangle {

        id: remoteHeader;
        height: 22;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        color: "#21252B";

        UiLabel {
            text: "Update settings";
        }
    }

    Rectangle {
        anchors.left: parent.left;
        anchors.right: parent.right;

        anchors.bottom: remoteHeader.bottom;
        height: 1;
        color: "#181A1F";
    }

    Row {
        anchors.centerIn: parent

        Column {
            Text {
               font.pointSize: 12;
               color: controller.updateAvailable ? "#2B68A4" : "#E74C3C";
               width: Math.min(400, contentWidth)
               text: controller.updateAvailable ? qsTr("Updates available") :
                                                  qsTr("No updates available");
               wrapMode: Text.WordWrap
            }
        }
    }

    Rectangle {
        id: updateFooter;
        height: 22;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        color: "#21252B";

        Row {
            anchors.right: parent.right;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.leftMargin: 8;
            spacing: 4;

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                label.text: controller.updateAvailable ? "Update" : "Check for updates";
                onClicked: {
                    if (!controller.updateAvailable) {
                        controller.onCheckForUpdates();
                    } else {
                        controller.onUpdate();
                        updatePopup.close();
                    }

                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                label.text: "Cancel";
                onClicked: {
                    updatePopup.close();
                }
            }

        }

        Row {
            anchors.left: parent.left;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;

            UiCheckbox {
                label.text: "Check for updates";
                from: controller;
                bind: "checkUpdate";
            }
        }
    }
}
