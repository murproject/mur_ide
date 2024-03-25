import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Layouts 1.12

Popup {
    id: popup;
    x: (parent.x + parent.width - popup.width) + 1;
    y: (parent.y + parent.height * 2) / 2 - 1;
    closePolicy: Popup.NoAutoClose;
    visible: !Controllers.scripts.local;
    padding: 10;
    width: Math.max(contentWidth + padding * 2, 135);
    font.family: Style.fontMono;
    font.pointSize: 10;
    opacity: fullWindow.visible ? 0.8 : 1.0;

    background: Rectangle {
        anchors.fill: parent;
        color: Style.bgDark;
        border.width: 1;
        border.color: Style.bgDarker;
        opacity: fullWindow.visible ? 0.75 : 1.0;
    }

    ColumnLayout {
        anchors.fill: parent;

        Label {
            text: "Telemetry";
            color: fullWindow.visible ? Style.white : Style.lightGray;
            font.family: Style.fontMono;
        }

        GridLayout {
            Layout.fillWidth: true;
            columns: 2;
            rowSpacing: 2;
            columnSpacing: 4;

            TelemetryGroup {
                model: [
                    "Yaw:", Controllers.network.yaw.toFixed(2),
                    "Pitch:", Controllers.network.pitch.toFixed(2),
                    "Roll:", Controllers.network.roll.toFixed(2),
                ];
            }

            TelemetryGroup {
                visible: !Controllers.network.usv;
                model: [
                    "Depth:", Controllers.network.depth.toFixed(2),
                    "Temp:", Controllers.network.temperature.toFixed(2),
                ];
            }

            TelemetryGroup {
                visible: Controllers.network.usv;
                model: [
                    "Latitude:", Controllers.network.latitude.toFixed(2),
                    "Longitude:", Controllers.network.longitude.toFixed(2),
                    "Satellites:", Controllers.network.satellites.toFixed(0),
                    "Altitude:", Controllers.network.altitude.toFixed(2),
                    "Speed:", Controllers.network.speed.toFixed(2),
                ];
            }

            TelemetryGroup {
                visible: Controllers.network.rov;
                model: [
                    "FCU 0", "",
                    "- Volt:", Controllers.network.fcu_telemetry[0].toFixed(2),
                    "- Amp:", Controllers.network.fcu_telemetry[1].toFixed(2),

                    "FCU 1", "",
                    "- Volt:", Controllers.network.fcu_telemetry[2].toFixed(2),
                    "- Amp:", Controllers.network.fcu_telemetry[3].toFixed(2),
                ];
            }

            TelemetryGroup {
                visible: !Controllers.network.rov;
                model: [
                    "Battery:", Controllers.network.battery.toFixed(0) + "%",
                ];
                color: Style.batteryColor;
            }
        }

        Rectangle {
            width: 60;
            height: width;
            Layout.alignment: Qt.AlignHCenter;
            color: "transparent";
            border.width: 2;
            border.color: "#44FFFFFF";
            clip: true;

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                height: parent.height - 4;
                width: 2;
                color: "#44FFFFFF";
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                width: parent.width - 4;
                height: 2;
                color: "#44FFFFFF";
            }

            Rectangle {
                id: pitcher;
                y: parent.height / 2 - height / 2 - Controllers.network.pitch / 2;
                anchors.horizontalCenter: parent.horizontalCenter;
                width: parent.width / 1.5;
                height: width / 8;
                color: "white";
                opacity: 0.5;
                antialiasing: true;
                radius: 3;
                rotation: Controllers.network.roll;

                Behavior on rotation {
                   NumberAnimation {
                       duration: Style.animFastest;
                       easing.type: Easing.InOutQuart;
                   }
               }

               Behavior on y {
                   NumberAnimation {
                       duration: Style.animFastest;
                       easing.type: Easing.InOutQuart;
                   }
               }
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter;
                y: parent.height / 2 - height / 2 - Controllers.network.pitch / 2;
                width: 5;
                height: 5;
                radius: 5;
                color: "white";
                opacity: 0.75;

                Behavior on y {
                    NumberAnimation {
                        duration: Style.animFastest;
                        easing.type: Easing.InOutQuart;
                    }
                }
            }
        }

    }

    enter: Transition {
        NumberAnimation {
            property: "x";
            from: x + width;
            to: x;
            easing.type: Style.animEasing;
            duration: Style.animDuration;
        }
    }
}



