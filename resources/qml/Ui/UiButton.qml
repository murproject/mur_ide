import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle {
    id: root;
    width: frameless ? row.width + 6 : row.width + 20;
    height: row.height + 6;

    property alias label: label;
    property alias labelColor: label.color;
    property alias iconColor: icon.color;
    property alias iconSize: icon.font.pointSize;
    property alias icon: icon.icon;
    property alias icons: icon.icons;
    property bool highlight: false;
    property bool frameless: false;
    property string toolTip: "";

    color: frameless ? "#00000000" : highlight ? "#2B68A4" : ma.pressed ? "#181A1F" : ma.containsMouse ? "#363C46" : "#333842"
    border.color: frameless ? "#00000000" : "#181A1F"
    radius: 2;

    signal clicked;

    Row {
        id: row;
        anchors.centerIn: parent;
        spacing: 4;

        Icon {
            id: icon;
            anchors.verticalCenter: parent.verticalCenter;
            color: label.color;
        }

        UiLabel {
            id: label;
            anchors.verticalCenter: parent.verticalCenter;
            font.weight: Font.Medium;
            color: enabled ? ma.pressed ? "#1D9FF2" : highlight || ma.containsMouse ? "#fff" : "#9DA5B4" : "#6E7582";
        }
    }

    MouseArea {
        id: ma;
        anchors.fill: parent;
        propagateComposedEvents: true;
        hoverEnabled: true;

        onClicked: {
            root.forceActiveFocus();
            root.clicked();
        }
    }

    ToolTip {
        visible: ma.containsMouse && root.toolTip.length > 0;
        text: qsTr(root.toolTip);
        delay: 1000
        timeout: 5000
    }


}
