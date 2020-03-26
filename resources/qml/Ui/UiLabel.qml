import QtQuick 2.2

Text {
    color: enabled ? "#9DA5B4" : "#6E7582"

    font.family: "Segoe WPC";
    font.pointSize: 10;
    //renderType: TextEdit.NativeRendering;

    elide: Text.ElideRight;
    textFormat: Text.PlainText;
    horizontalAlignment : Text.AlignLeft;
}
