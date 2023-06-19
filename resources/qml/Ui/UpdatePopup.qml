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
    height: 200;
    property var controller: Controllers.updates;

    background: Rectangle {
        anchors.fill: parent;
        color: Style.bgDark;
        border.width: 1;
        border.color: Style.bgDarker;
    }

    Rectangle {
        id: remoteHeader;
        height: 22;
        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        color: Style.bgDark;

        UiLabel {
            text: "Update settings";
        }
    }

    Rectangle {
        anchors.left: parent.left;
        anchors.right: parent.right;

        anchors.bottom: remoteHeader.bottom;
        height: 1;
        color: Style.bgDarker;
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

    Column {
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            bottomPadding: 8;
            UiCheckbox {
                label.text: "Check for updates on start";
                from: controller;
                bind: "checkUpdate";
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 4;

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                label.text: controller.updateAvailable ? "Update" : "Check for updates";
                // disable updates
//                onClicked: {
//                    if (!controller.updateAvailable) {
//                        controller.onCheckForUpdates();
//                    } else {
//                        controller.onUpdate();
//                        updatePopup.close();
//                    }
//                }
            }

            UiButton {
                anchors.verticalCenter: parent.verticalCenter;
                label.text: "Cancel";
                onClicked: {
                    updatePopup.close();
                }
            }
        }
    }
}
