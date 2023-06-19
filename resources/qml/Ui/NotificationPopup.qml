import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Popup {
    id: notificationPopup;
    property string type: "info"; // ok, info, warn, error
    property string headerText: "";
    property string messageText: "";
    property string customIcon: "";
    property bool mini: false;
    property bool disappear: false;
    property int delay: 5000;
    focus: false;

    property color accentColor: {
        if (type === "ok") {
            accentColor: Style.green;
        } else if (type === "info") {
            accentColor: Style.blue;
        } else if (type === "warn" || type === "warning") {
            accentColor: Style.orangeDark;
        } else if (type === "error") {
            accentColor: Style.red;
        } else {
            accentColor: Style.semiDarker;
        }
    };

    x: (parent.width - width) + 1;
    y: spliter.y + spliter.height - height * 2.0;

    width: mini ? 42: 300;
    padding: 12;

    parent: ApplicationWindow.overlay;
    visible: true;

    Timer {
        interval: notificationPopup.delay;
        running: notificationPopup.disappear;
        repeat: false;
        onTriggered: notificationPopup.close();
    }

    background: Rectangle {
        id: back;
        anchors.fill: parent;
        color: Style.semiDarker;
        border.width: 1;
        border.color: Style.bgDarker;

        SequentialAnimation on border.color {
            alwaysRunToEnd: true;
            loops: 1;
            PropertyAnimation { from: accentColor; to: back.border.color; duration: 2000; }
        }
    }

    enter: Transition {
        NumberAnimation {
            property: "x";
            from: x + width;
            to: x;
            easing.type: Style.animEasing;
            duration: Style.animDuration;
        }
        NumberAnimation {
            property: "opacity";
            from: 0.0;
            to: opacity;
            easing.type: Style.animEasing;
            duration: Style.animDuration;
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "x";
            from: x;
            to: x + width;
            easing.type: Style.animEasing;
            duration: Style.animDuration;
        }
        NumberAnimation {
            property: "opacity";
            from: opacity;
            to: 0.0;
            easing.type: Style.animEasing;
            duration: Style.animDuration;
        }
    }

    Column {
        id: container;
        anchors.fill: parent;

        RowLayout  {
            spacing: 6;
            anchors.left: parent.left;
            anchors.right: parent.right;

            Icon {
                icon: {
                    if (notificationPopup.customIcon !== "") {
                        icon: notificationPopup.customIcon;
                    } else {
                        if (notificationPopup.type === "ok") {
                            icon: icons.fa_check_circle;
                        } else if (notificationPopup.type === "info") {
                            icon: icons.fa_info_circle;
                        } else if (notificationPopup.type === "warn" || notificationPopup.type === "warning") {
                            icon: icons.fa_exclamation_triangle;
                        } else if (notificationPopup.type === "error") {
                            icon: icons.fa_exclamation_circle;
                        } else {
                            icon: icons.fa_info_circle;
                        }
                    }
                }
                font.pointSize: 12;
                width: 16;
                Layout.alignment: Qt.AlignVCenter;
            }

            UiLabel {
                id: header;
                visible: !notificationPopup.mini;
                font.pointSize: 14;
                text: notificationPopup.headerText;
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter;
            }

            UiButton {
                id: closeButton;
                visible: !notificationPopup.mini;
                frameless: true;
                Layout.alignment: Qt.AlignRight
                icon: icons.fa_times;
                toolTip: "Close";
                opacity: 0.7;
                onClicked: {
                    notificationPopup.close();
                }
            }
        }

        Row {
            leftPadding: 23;

            UiLabel {
                visible: !notificationPopup.mini;
                id: message;
                textFormat: Text.RichText;
                text: notificationPopup.messageText;
            }
        }

    }
}
