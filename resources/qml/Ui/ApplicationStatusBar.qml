import QtQuick 2.6

Rectangle {
    height: 24;
    color: "#21252B"

    Rectangle {
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;
        height: 1;
        color: "#181A1F"
    }

    Row {
        id: fileNameRow;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.leftMargin: 8;
        spacing: 4;

        Icon {
            anchors.verticalCenter: parent.verticalCenter;
            visible: Controllers.editor.fileUrl.length > 0;
            icon: icons.fa_file;
        }

        UiLabel {
            anchors.verticalCenter: parent.verticalCenter;

            text: Controllers.editor.fileUrl.length > 0 ? Controllers.editor.fileUrl : "Empty"
            font.pointSize: 10;
        }
    }

    Row {
        id: statusRow;

        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        anchors.rightMargin: 4;
        spacing: 4;

        /*
        Icon {
            anchors.verticalCenter: parent.verticalCenter;
            icon: Controllers.network.battery >= 70 ? icons.fa_battery_full : Controllers.network.battery >= 50 ? icons.fa_battery_half : Controllers.network.battery >= 20 ? icons.fa_battery_quarter : icons.fa_battery_empty;
            color: Controllers.network.connected ? Controllers.network.battery < 20 ? "#E74C3C" : "#148F77" : "#626567";
        }

        Icon {
            anchors.verticalCenter: parent.verticalCenter;
            icon: icons.fa_circle;
            color: Controllers.network.connected ? "#148F77" : "#E74C3C";
        }
        */
        UiLabel {
            anchors.verticalCenter: parent.verticalCenter;
            text: "version: " + "0.0.5";
        }
    }
}
