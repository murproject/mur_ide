import QtQuick 2.6

Rectangle {
    height: 24;
    color: Style.bgDark

    Rectangle {
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
        height: 1;
        color: Style.bgDarker;
    }

    Row {
        id: fileNameRow;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.leftMargin: 8;
        spacing: 4;

        Icon {
            anchors.verticalCenter: parent.verticalCenter;
            visible: Controllers.editor.fileUrl.length > 0;
            icon: icons.fa_file;
        }

        UiLabel {
            anchors.verticalCenter: parent.verticalCenter;
            text: Controllers.editor.fileUrl.length > 0 ? Controllers.editor.fileUrl : "Empty"
            width: root.width * 0.7;
            elide: Text.ElideLeft;
            font.pointSize: 10;
        }
    }

    Row {
        id: statusRow;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        anchors.rightMargin: 4;
        spacing: 4;

        UiLabel {
            anchors.verticalCenter: parent.verticalCenter;
            text: "Version: " + Controllers.version;
        }
    }
}
