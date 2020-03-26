import QtQuick 2.12
import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2

Rectangle {
    id: root;

    color: "#fff";
    width: childrenRect.width;
    height: 40;

    ListView {
        id: listView;
        implicitWidth: contentItem.childrenRect.width;
        implicitHeight: contentHeight;
        boundsBehavior: Flickable.StopAtBounds;
        //            text: modelData;

        model: Controllers.logger.entries;
        delegate: Rectangle {
            color: "#fff";
            UiLabel {
                text: modelData;
                font.pointSize: 9;
            }
        }
        onCountChanged: {
            listView.positionViewAtEnd()
        }
    }
}
