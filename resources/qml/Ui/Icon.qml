import QtQuick 2.2

import "fontawesome.js" as FA

Text {
    id: root;

    property var icons: FA.Icons;
    property alias icon: root.text;
    property alias pointSize: root.font.pointSize;

    font.family: "FontAwesome";
    font.pointSize: 13;
    style: Text.Normal;
    color: enabled ? Style.lightGray : Style.darkGray;
    textFormat: Text.PlainText;
    verticalAlignment: Text.AlignVCenter;
    horizontalAlignment: Text.AlignHCenter;
    renderType: Text.NativeRendering;

    Behavior on color {
        ColorAnimation {
            duration: Style.animFastest;
            easing.type: Style.animEasing;
        }
    }
}
