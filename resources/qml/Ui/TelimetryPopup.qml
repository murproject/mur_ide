import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4

Popup {
    id: popup;
//    x: (parent.x + parent.width - popup.width) / 2;
//    y: (parent.y + parent.height * 2) / 2 - 1;
      x: (parent.x + parent.width - popup.width);
      y: (parent.y + parent.height * 2) / 2 - 1;
    closePolicy: Popup.NoAutoClose;
    visible: true;

    background: Rectangle {
        anchors.fill: parent;
        color: "#21252B";
        border.width: 1;
        border.color: "#181A1F";
    }
    Row {
        Column {
            Label {
                bottomPadding: 3;
                text: "Telemetry";
                font.pointSize: 9;
                color: "#9DA5B4";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text: "Yaw: ";/// + Controllers.network.yaw.toFixed(2);
                color: "#F39C12";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text: "Pitch: ";// + Controllers.network.pitch.toFixed(2);
                color: "#F39C12";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text: "Roll: ";// + Controllers.network.roll.toFixed(2);
                color: "#F39C12";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text: "Depth: ";// + Controllers.network.depth.toFixed(2);
                color: "#F39C12";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text: "Battery: ";// + Controllers.network.pressure.toFixed(2);
                color: Controllers.network.connected ? Controllers.network.battery < 20 ? "#E74C3C" : "#148F77" : "#626567";
            }
        }

        Column {
            Label {
                bottomPadding: 3;
                text: " ";
                font.pointSize: 9;
                color: "#9DA5B4";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text:  Controllers.network.yaw.toFixed(2);
                color: "#F39C12";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text: Controllers.network.pitch.toFixed(2);
                color: "#F39C12";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text:  Controllers.network.roll.toFixed(2);
                color: "#F39C12";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text: Controllers.network.depth.toFixed(2);
                color: "#F39C12";
            }
            Label {
                topPadding: 2;
                bottomPadding: 2;
                text: Controllers.network.battery.toFixed(0) + "%";
                color: Controllers.network.connected ? Controllers.network.battery < 20 ? "#E74C3C" : "#148F77" : "#626567";
            }
        }
    }
/*
    Column {
        Label {
            bottomPadding: 3;
            text: "Telemetry";
            font.pointSize: 9;
            color: "#9DA5B4";
        }
        Label {
            topPadding: 2;
            bottomPadding: 2;
            text: "Yaw: " + Controllers.network.yaw.toFixed(2);
            color: "#F39C12";
        }
        Label {
            topPadding: 2;
            bottomPadding: 2;
            text: "Pitch: " + Controllers.network.pitch.toFixed(2);
            color: "#F39C12";
        }
        Label {
            topPadding: 2;
            bottomPadding: 2;
            text: "Roll: " + Controllers.network.roll.toFixed(2);
            color: "#F39C12";
        }
        Label {
            topPadding: 2;
            bottomPadding: 2;
            text: "Depth: " + Controllers.network.depth.toFixed(2);
            color: "#F39C12";
        }
        Label {
            topPadding: 2;
            bottomPadding: 2;
            text: "Pressure: " + Controllers.network.pressure.toFixed(2);
            color: "#F39C12";
        }
    }
    */
}



