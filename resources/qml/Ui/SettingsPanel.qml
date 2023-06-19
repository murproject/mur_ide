import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Popup {
    id: settingsPopup;
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: parent.width / 1;
    height: parent.height / 1;
    closePolicy: Popup.CloseOnEscape;
    modal: true;
    visible: true;
    parent: ApplicationWindow.overlay;

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
        anchors.bottom: parent.bottom;
        width: Math.min(parent.width, 900);
        anchors.horizontalCenter: parent.horizontalCenter;

        Row {
            Layout.alignment: Qt.AlignHCenter;

            UiLabel {
                anchors.verticalCenter: parent.verticalCenter;
                font.pointSize: Style.headerFontSize;
                text: settingsTabs.visibleItem.tabTitle + " settings";
            }
        }

        Rectangle {
            Layout.fillWidth: true;
            width: parent.width;
            height: 1;
            color: Style.bgBlue;
        }

        TabView {
            id: settingsTabs;
            Layout.fillWidth: true;
            Layout.fillHeight: true;

            SettingsRemote {
                id: settingsRemote;
            }

            SettingsNetwork {
                id: settingsNetwork;
            }

            SettingsGamepad {
                id: settingsGamepad;
            }
        }
    }

    Row {
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;

        UiButton {
            id: settingsOkButton;
            anchors.verticalCenter: parent.verticalCenter;
            label.text: "OK";
            label.font.bold: true;
            onClicked: {
                settingsGamepad.save();
                settingsPopup.close();
            }
            visible: !settingsTabs.visibleItem.restartRequired;
        }

        UiButton {
            id: settingsRestartButton;
            anchors.verticalCenter: parent.verticalCenter;
            label.text: "Save";
            toolTip: "You may need to restart";
            label.font.bold: true;
            visible: settingsTabs.visibleItem.restartRequired;
            onClicked: {
                settingsOkButton.click();
                settingsRemote.save();
                settingsNetwork.save();
            }
        }
    }

    onVisibleChanged: {
        if (visible) {
            settingsRemote.load();
        }
    }
}
