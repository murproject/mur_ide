import QtQuick 2.4

Rectangle {
    id: root;

    height: 22;

    color: Style.bgDark

    UiLabel {
        anchors.verticalCenter: parent.verticalCenter;
        anchors.left: parent.left;
        anchors.leftMargin: 4;

        text: (Controllers.editor.selection.startLine + 1) + ":" + (Controllers.editor.selection.startColumn + 1);
        font.pointSize: 9;
    }

    Row {
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        anchors.rightMargin: 8;
        visible: parent.width > 80;

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;

            frameless: true;
            iconSize: 12;
            icon: icons.fa_search_plus;

            onClicked: Controllers.editor.increaseFontSize();
        }

        UiButton {
            anchors.verticalCenter: parent.verticalCenter;

            frameless: true;
            iconSize: 12;
            icon: icons.fa_search_minus;

            onClicked: Controllers.editor.decreaseFontSize();
        }
    }

    Rectangle {
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;

        height: 1;
        color: Style.bgDarker
    }
}
