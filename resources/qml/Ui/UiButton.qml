import QtQuick 2.9
import QtQuick.Controls 2.2


Rectangle {
    id: root;
    width: frameless ? row.width + 6 : row.width + 20;
    height: row.height + 6;

    property alias label: label;
    property alias labelColor: label.color;
    property alias iconColor: icon.color;
    property alias iconOpacity: icon.opacity;
    property alias iconSize: icon.font.pointSize;
    property alias iconRotation: icon.rotation;
    property alias icon: icon.icon;
    property alias icons: icon.icons;
    property alias after_item: after_item;
    property alias shortcut: shortcut;
    property alias ma: ma;
    property bool highlight: false;
    property bool frameless: false;
    property string toolTip: "";
    property bool outline: fullWindow.visible ? true : false;
    property bool setFocus: true;

    opacity: visible ? 1.0 : 0.0;

    color: frameless ? Style.transparent : highlight ? Style.blue : ma.pressed ? Style.semiDarkest : ma.containsMouse ? Style.semiDark : Style.semiDarker;
    border.color: frameless ? Style.transparent : Style.bgDarker;
    radius: 2;

    signal clicked;
    signal pressing;

    Row {
        id: row;
        anchors.centerIn: parent;
        spacing: 6;

        Icon {
            id: icon;
            anchors.verticalCenter: parent.verticalCenter;
            color: label.color;
            width: text.length > 0 ? font.pixelSize : 0;
            horizontalAlignment: Text.AlignHCenter;
            verticalAlignment: Text.AlignVCenter;
            style: Text.Outline;
            styleColor: Style.outlineColor;
        }

        Item {
            id: after_item;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
        }

        UiLabel {
            id: label;
            anchors.verticalCenter: parent.verticalCenter;
            font.weight: Font.Medium;
            color: enabled ? ma.pressed ? Style.lightBlue : highlight || ma.containsMouse ? Style.white : Style.lightGray : Style.darkGray;

            style: outline ? Text.Outline : Text.Normal;
            styleColor: Style.outlineColor;
            font.bold: outline;
        }
    }


    Timer {
        id: repeatedPressing;
        repeat: true;
        running: false;
        interval: 100;

        onTriggered: {
            root.pressing();
        }
    }

    MouseArea {
        id: ma;
        anchors.fill: parent;
        propagateComposedEvents: true;
        hoverEnabled: true;

        onClicked: {
            if (setFocus) {
                root.forceActiveFocus();
            }
            root.clicked();
        }

        onPressedChanged: {
            repeatedPressing.running = pressed;
        }
    }

    ToolTip {
        visible: ma.containsMouse && root.toolTip.length > 0;
        text: qsTr(root.toolTip);
        delay: 1000
        timeout: 5000
        palette.toolTipBase: Style.white;
        palette.toolTipText: Style.bgDarker;
    }

    Shortcut {
        id: shortcut;
        autoRepeat: false;
        context: Qt.ApplicationShortcut;
        onActivated: {
            if (root.enabled) {
                root.clicked()
            }
        }
    }

    function click() {
        root.clicked();
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 500;
            easing.type: Style.animEasing;
        }
    }

    Behavior on width {
        NumberAnimation {
            duration: 250;
            easing.type: Style.animEasing;
        }
    }

    Behavior on color {
        ColorAnimation {
            duration: Style.animFastest;
            easing.type: Style.animEasing;
        }
    }
}
