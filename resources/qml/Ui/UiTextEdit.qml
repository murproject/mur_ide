import QtQuick 2.6
import QtQuick.Layouts 1.12

Item {
    id: root;
    Layout.fillWidth: true;
    Layout.alignment: Qt.AlignTop;
    Layout.preferredHeight: edit.height;
    property alias wrapMode: edit.wrapMode;
    property alias text: edit.text;
    Layout.bottomMargin: 4;

    Rectangle {
        width: edit.width;
        height: edit.height + edit.padding;
        color: Style.bgDarker;
        border.color: Style.semiDarkest;
        border.width: 1;
    }

    ApplicationTextEdit {
        id: edit;
        Layout.fillWidth: true;
        height: contentHeight;
        width: parent.width;
        font.pointSize: 10;
        font.family: Style.fontMono;
        color: enabled ? Style.lighterGray : Style.darkGray;
        wrapMode: "WordWrap";
        padding: 4;
        topPadding: 2;
        bottomPadding: 0;
    }
}
