import QtQuick 2.11

Rectangle {
    id: root;

    width: label.width;
    height: label.height;

    property alias label: label;

    color: "#008800";
    border.color: Style.bgDarker

    radius: 2;

    UiLabel {
        id: label;

        anchors.centerIn: parent;
        font.weight: Font.Medium;
        font.pointSize: 11;
        color: "#fff"
    }
}
