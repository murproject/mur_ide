import QtQuick 2.12
import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2

Rectangle {
    id: root;

    color: Style.bgBlue;
    property string tabTitle: "Console";

    ScrollView {
        id: logScrollView;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.bottom: footer.top;
        rightPadding: 2;
        bottomPadding: 8;
        clip: true;
        ScrollBar.horizontal.policy: buttonWordWrap.active ? ScrollBar.AlwaysOff : ScrollBar.AsNeeded;

        TextArea {
            id: outputArea;
            renderType: TextArea.NativeRendering;
            topPadding: 2;
            leftPadding: 4;
            rightPadding: 0;
            bottomPadding: 4;
            readOnly: true;
            color: Style.orange;
            font.family: Style.fontMono;
            font.pixelSize: 12;
            textFormat: Text.RichText;
            text: Controllers.logger.output;
            wrapMode: buttonWordWrap.active ? Text.WordWrap : Text.NoWrap;
            width: buttonWordWrap.active ? root.width : parent.width;
            selectByMouse: true;
            mouseSelectionMode: TextArea.SelectCharacters;
            selectionColor: Style.semiDark;
            selectedTextColor: Style.lighterGray;

            onTextChanged: {
                if (outputArea.height > logScrollView.height) {
                    outputArea.cursorPosition = outputArea.length-1;
                }
            }
        }
    }


    UiLabel {
        id: placeholder;
        anchors.fill: parent;
        text: "Console output...";
        font.pixelSize: 18;
        verticalAlignment: Text.AlignVCenter;
        horizontalAlignment: Text.AlignHCenter;
        textFormat: Text.RichText;
        opacity: outputArea.length > 0 || parent.width < 200 ? 0 : 0.5;
        clip: true;

        Behavior on opacity {
            NumberAnimation {
                duration: Style.animFast;
            }
        }
    }

    Rectangle {
        id: footer;
        height: 21;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        color: Style.bgDark;
        property bool shrink: width < 200;

        Row {
            anchors.right: parent.right;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.rightMargin: 4;
            spacing: 4;
            visible: footer.width > 80;

            UiButton {
                id: buttonWordWrap;
                property bool active: false;
                anchors.verticalCenter: parent.verticalCenter;
                icon: active ? icons.fa_indent : icons.fa_align_justify;
                label.text: footer.shrink ? "" : "Word wrap";
                highlight: active;
                onClicked: {
                    active = !active;
                    logScrollView.ScrollBar.horizontal.position = 0;
                }
            }

            UiButton {
                id: buttonClearOutput;
                icon: icons.fa_trash_o;
                label.text: footer.shrink ? "" : "Clear";
                anchors.verticalCenter: parent.verticalCenter;
                onClicked: Controllers.logger.clear();
            }
        }
    }

}
