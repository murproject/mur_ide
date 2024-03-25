import QtQuick 2.2


Text {
    color: enabled ? Style.lightGray : Style.gray;

    font.family: Style.fontSans;
    font.pointSize: 10;

    elide: Text.ElideRight;
    textFormat: Text.PlainText;
    horizontalAlignment : Text.AlignLeft;
}
