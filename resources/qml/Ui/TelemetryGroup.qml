import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Layouts 1.12

Repeater {
    id: group;
    property var color: fullWindow.visible ? Style.white : Style.orange;

    Label {
        id: label;
        property bool onRight: index % 2 == 1;
        Layout.fillWidth: index % 2 == 1;
        horizontalAlignment: onRight ? Text.AlignRight : Text.AlignLeft;
        visible: group.visible;
        text: modelData;

        topPadding: 2;
        bottomPadding: 2;
        color: group.color;
        font.family: Style.fontMono;
    }
}

