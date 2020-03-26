import QtQuick 2.12
import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2

Rectangle {
    id: root;

    color: "#282C34";

    property string tabTitle: Controllers.logger.entries.length > 0 ? "Messages <b>" + Controllers.logger.entries.length + "</b>" : "Console"

    ListView {
        id: listView;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
        anchors.bottom: footer.top;
        ScrollBar.vertical: ScrollBar {
            minimumSize: 0.2;
        }
        ScrollBar.horizontal: ScrollBar {
            minimumSize: 0.2;
        }
        boundsBehavior: Flickable.StopAtBounds;

        model: Controllers.logger.entries;
        delegate: Label {
            anchors.leftMargin: 4;
            anchors.left: parent.left;
            anchors.right: parent.right;
            color: "#F39C12";
            text: modelData;
        }
        onCountChanged: {
            listView.positionViewAtEnd()
        }
    }

    Rectangle {
        id: footer;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;

        height: footerCol.height;

        color: "#282C34";

        Column {
            id: footerCol;

            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.bottom: parent.bottom;

            Rectangle {
                anchors.left: parent.left;
                anchors.right: parent.right;
                height: 1;

                color: "#21252B";
            }

            UiButton {
                anchors.right: parent.right;
                anchors.rightMargin: 4;
                icon: icons.fa_trash_o;
                label.text: "Clear"
                visible: parent.width > 70;
                onClicked: Controllers.logger.clear();
            }
        }
    }
}
