import QtQuick 2.9

Item {
    id: root;

    width: row.width;
    height: label.height + 4;

    property alias label: label;
    property bool checked: false;
    property var from: root;
    property string bind: "checked";

    signal clicked;

    Row {
        id: row;
        anchors.left: parent.left;
        anchors.top: parent.top;
        spacing: 4;

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter;
            width: 16;
            height: 16;
            color: ma.pressed ? Style.bgDarker : ma.containsMouse ? "#363C46" : "#333842"
            border.color: Style.bgDarker;
            radius: 2;

            Icon {
                anchors.centerIn: parent;

                color: label.color;
                visible: root.checked;
                icon: icons.fa_check;
            }
        }

        UiLabel {
            id: label;
            anchors.verticalCenter: parent.verticalCenter;
            font.weight: Font.Medium;
            color: enabled ? ma.containsMouse ? "#fff" : "#9DA5B4" : Style.gray;
        }
    }

    MouseArea {
        id: ma;
        anchors.fill: parent;
        propagateComposedEvents: true;
        hoverEnabled: true;

        onClicked: {
            root.clicked();
            if (root.from) {
                root.from[root.bind] = !root.from[root.bind];
            }
        }
    }

    Component.onCompleted: {
        root.checked = Qt.binding(function() { return root.from[root.bind]; });
    }
}
