import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4
import Hints 1.0

Rectangle {
    id: root;

    property var editor: Controllers.editor;

    clip: true;

    color: Style.bgDarker

    function activate() {
        textEdit.forceActiveFocus();
    }

    Flickable {
        id: flickable;

        anchors.left: lines.right;
        anchors.top: parent.top;
        anchors.bottom: search.visible ? search.top : footer.top;
        anchors.right: parent.right;

        contentWidth: textEdit.width;
        contentHeight: textEdit.height;

        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar { }
        ScrollBar.horizontal: ScrollBar { }

        maximumFlickVelocity : 1100.0;

        function updateScrollX(x) {
            if (contentX >= x) {
                contentX = x;
            } else if (contentX + width <= x) {
                contentX = x + 1 - width;
            }
        }

        function updateScrollY(y) {
            if (contentY >= y) {
                contentY = y;
            } else if (contentY + height <= y + textEdit.cursorHeight) {
                contentY = y + textEdit.cursorHeight - height;
            }
        }

        Rectangle {
            id: cursorBg;

            anchors.left: parent.left;
            anchors.right: parent.right;

            y: textEdit.cursorY;
            height: textEdit.cursorHeight;

            color: Style.bgDark
        }

        Repeater {
            model: Controllers.editor.errors;

            delegate: CodeEditorError {
                anchors.left: parent.left;
                anchors.right: parent.right;

                height: textEdit.cursorHeight;
                y: (model.line - 1) * height;

                error: model;
            }
        }

        ApplicationTextEdit {
            id: textEdit

            width: Math.max(implicitWidth, root.width - lines.width + 1);
            height: Math.max(implicitHeight, root.height - footer.height);
            leftPadding: 4;
            font.pointSize: editor.fontSize;

            property bool multiSelection: editor.selection.startLine !== editor.selection.endLine;
            property int cursorX: cursorRectangle.x;
            property int cursorY: cursorRectangle.y;
            property int cursorHeight: cursorRectangle.height;
            property bool showCursor: true;

            Keys.onPressed: {
                if ((event.key === Qt.Key_Z || event.key === Qt.Key_Y) &&
                    (Qt.ShiftModifier && Qt.ControlModifier || Qt.ControlModifier))
                {
                    editor.indenter.setIndentationBlock(true);
                } else {
                    editor.indenter.setIndentationBlock(false);
                }

                if (event.key === Qt.Key_Tab) {
                    editor.hints.applyHint(Hints.HINT_INSERT_INDENT);
                    event.accepted = true;
                }

                if (event.key === Qt.Key_Backtab) {
                    editor.hints.applyHint(Hints.HINT_REMOVE_INDENT);
                    event.accepted = true;
                }

                if (event.key === Qt.Key_Slash && event.modifiers === Qt.ControlModifier) {
                    editor.hints.applyHint(Hints.HINT_COMMENT);
                    event.accepted = true;
                }
            }

            onActiveFocusChanged: {
                if (activeFocus) {
                    activateCursor();
                } else {
                    deactivateCursor();
                }
            }


            onCursorXChanged: {
                activateCursor();
                flickable.updateScrollX(cursorX);
            }

            onCursorYChanged: {
                activateCursor();
                flickable.updateScrollY(cursorY);
            }

            cursorDelegate: Rectangle {
                id: cursorDel;
                width: 1.5;
                color: "orange";
                visible: textEdit.showCursor && !textEdit.multiSelection;
                antialiasing: true;
                smooth: true;
            }

            MouseArea {
                anchors.fill: parent;
                enabled: !textEdit.activeFocus;
                cursorShape: Qt.IBeamCursor;
                onClicked: {
                    root.activate();
                }
            }

            function activateCursor() {
                textEdit.showCursor = true;
                cursorTimer.restart();
            }

            function deactivateCursor() {
                textEdit.showCursor = false;
                cursorTimer.stop();
            }

            Timer {
                id: cursorTimer;
                interval: 535;// Controllers.settings.cursorFlashTime;
                running: true;
                repeat: true;
                onTriggered: textEdit.showCursor = !textEdit.showCursor;

            }
        }
    }

    Rectangle {
        id: lines;

        anchors.left: parent.left;
        anchors.top: parent.top;
        anchors.bottom: footer.top;

        color: Style.bgDarker

        width: linesCol.width + 4;

        Column {
            id: linesCol;

            y: -flickable.contentY;

            Repeater {
                model: textEdit.lineCount;

                delegate: Text {
                    anchors.right: parent.right;
                    rightPadding: 8;
                    leftPadding: 8;

                    font.family: textEdit.font.family;
                    font.pointSize: editor.fontSize;
                    verticalAlignment: Text.AlignVCenter;

                    color: index  >= editor.selection.startLine && index <= editor.selection.endLine ? Style.white : Style.gray
                    text: index + 1;
                }
            }
        }

        Rectangle {
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;

            width: 1;
            color: Style.bgDark
        }
    }

    CodeEditorSearch {
        id: search;

        anchors.left: parent.left;
        anchors.bottom: footer.top;
        anchors.right: parent.right;

        refocus: textEdit;
    }

    CodeEditorFooter {
        id: footer;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
    }

    Connections {
        target: editor;
        function onClear () {
            textEdit.clear();
        }
        function onSelect() {
            textEdit.select(start, end);
        }
    }

    Component.onCompleted: {
        editor.document = textEdit.textDocument;
        editor.cursorX = Qt.binding (function() { return textEdit.cursorX; });
        editor.cursorY = Qt.binding (function() { return textEdit.cursorY; });
        editor.selection.startPosition = Qt.binding (function() { return textEdit.selectionStart; });
        editor.selection.endPosition = Qt.binding (function() { return textEdit.selectionEnd; });
        textEdit.forceActiveFocus();
    }
}
