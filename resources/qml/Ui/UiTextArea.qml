import QtQuick 2.6
import QtQuick.Controls 2.12

TextArea {
    id: root;

    property var refocus;
    property var from;

    property string bind;
    property string lastInput;
    property string newInput;

    property bool immediate: false;
    property bool resetInput: false;
    property bool clearFocusOnEnter: true;

    property alias radius: rect.radius;

    property color borderDefault: Style.bgDarker
    property color borderActive: "#9DA5B4"

    leftPadding: 4;
    rightPadding: 4;

    color: enabled ? "#fff" : "#9DA5B4";
    selectedTextColor: "#000";
    selectionColor: root.borderActive;

    font.family: Style.fontMono;
    font.pointSize: 13;
    verticalAlignment: TextInput.AlignVCenter;

    activeFocusOnPress: true;
    selectByMouse: true;
    clip: true;

    Rectangle {
        id: rect;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.verticalCenter: parent.verticalCenter;

        z: parent.z - 1;
        height: root.contentHeight;

        color: "#333842"
        border.color: root.activeFocus ? root.borderActive : root.borderDefault;
    }

    onActiveFocusChanged: {
        if (activeFocus) {
            root.lastInput = text;
        } else {
            if (root.resetInput) {
                applyText(newInput);
            } else {
                applyText(text);
            }
        }
    }

    onTextChanged: {
        if (root.immediate) {
            applyText(text);
        }
    }

    function applyText(newText) {
        if (root.from) {
            text = newText;
            root.from[root.bind] = newText;
        }
    }

    function clearFocus() {
        if (refocus) {
            refocus.forceActiveFocus();
        } else {
            root.parent.forceActiveFocus();
        }
    }

    Keys.onPressed: {
        var key = event.key;

        if (key === Qt.Key_Escape) {
            Controllers.editor.search.visible = false;
            event.accepted = true;
            refocus.forceActiveFocus();
        }
        else if ((key === Qt.Key_Enter || key === Qt.Key_Return) && clearFocusOnEnter === true) {
            enterPressed();
            event.accepted = true;
        }
    }
}
