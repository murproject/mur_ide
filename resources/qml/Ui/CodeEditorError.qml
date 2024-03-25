import QtQuick 2.9

Rectangle {
    id: root;

    property var error: undefined;

    color: "#782A32";

    Rectangle {
        anchors.left: parent.left;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;

        width: 2;
        color: "#ff0000"
    }

    TagLabel {
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;

        label.text: error.message;
        label.font.pointSize: Controllers.editor.fontSize - 2;

        color: Style.bgDarker
        border.color: root.color;
    }
}
