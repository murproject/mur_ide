import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

TextEdit {
    renderType: TextEdit.NativeRendering;
    font.family: "Consolas";
    font.pointSize: 10;
    font.bold: false;
    selectByMouse: true
    mouseSelectionMode: TextEdit.SelectCharacters
    selectionColor: "#3d4856"
    color: "#fff"
    textFormat: TextEdit.PlainText
    wrapMode: TextEdit.NoWrap
}



