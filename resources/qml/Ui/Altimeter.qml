import QtQuick 2.12
import QtQuick.Layouts 1.12
import mur 1.0

Rectangle {
    id: altimeter;
    property real depth: Controllers.network.depth;
    Behavior on depth {
        NumberAnimation {
            id: depthAnim;
            alwaysRunToEnd: true;
            duration: 200;
            easing.type: Easing.Linear;
       }
    }

    anchors.left: parent.left;
    anchors.leftMargin: 30;
    anchors.verticalCenter: parent.verticalCenter;
    width: 40;
    height: Math.min(parent.height * 0.5, 300);
    color: "transparent";
    border.width: 1;
    border.color: "#70000000";

    Item {
        id: wrapper;
        anchors.fill: parent;
        clip: true;

        Rectangle {
            id: meter;
            property real parts: 8;
            property real strokes: 4 * parts;
            property real h: parent.height * 1.5;
            anchors.verticalCenter: parent.verticalCenter;
            height: h;
            width: parent.width;
            color: Style.outlineColor;

            Item {
                anchors.fill: parent;

                Repeater {
                    model: meter.strokes;

                    Rectangle {
                        width: 2 + parent.width / 4 * ((index + 3) % 2 * (index + 3) % 4);
                        anchors.horizontalCenter: parent.horizontalCenter;
                        y: (-altimeter.depth * 8 * meter.height / meter.strokes) % (meter.height / meter.parts * 2) + ((index + 4) * (meter.height / meter.strokes));

                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            width: parent.width;
                            height: 2;
                            color: "#FFFFFF";
                            opacity: 0.15;
                        }
                    }
                }

                Repeater {
                    model: meter.parts + 1;

                    Rectangle {
                        y: (-altimeter.depth * 2 * meter.height / meter.parts) % (meter.height / meter.parts * 2) + ((index) * (meter.height / meter.parts));
                        width: parent.width;
                        anchors.horizontalCenter: parent.horizontalCenter;

                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            height: meter.height / 6;
                            width: parent.width;
                            opacity: index % 2 === 0 ? 0.4 : 0.1;

                            gradient: Gradient {
                                orientation: Gradient.Vertical;
                                GradientStop { position: 0.0; color: "#00FFFFFF" }
                                GradientStop { position: 0.5; color: "#CCAAAAAA" }
                                GradientStop { position: 1.0; color: "#00FFFFFF" }
                            }
                        }

                        UiLabel {
                            property bool big: index % 2 === 0;
                            anchors.verticalCenter: parent.verticalCenter;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            property real offset: altimeter.depth >= 0 ? Math.floor(altimeter.depth) : Math.ceil(altimeter.depth);
                            property real val: (index - meter.parts / 2) / 2 + offset;
                            text: val;
                            font.pointSize: big ? 12 : 9;
                            font.bold: true;
                            color: "#FFFFFF";
                            opacity: 0.5;
                            style: Text.Outline;
                            styleColor: Style.outlineColor;
                        }
                    }
                }
            }
        }
    }

    Icon {
        anchors.right: altimeter.left;
        anchors.verticalCenter: altimeter.verticalCenter;
        icon: icons.fa_caret_right;
        font.pointSize: 16;
        color: Controllers.image.speedMode === 0 ? Style.gray : Controllers.image.speedMode === 2 ? Style.yellow : Style.white;
        style: Text.Outline;
        styleColor: Style.outlineColor;
    }

    Rectangle {
        anchors.verticalCenter: altimeter.verticalCenter;
        anchors.left: altimeter.left;
        anchors.right: altimeter.right;
        height: 50;

        gradient: Gradient {
            orientation: Gradient.Vertical;
            GradientStop { position: 0.0; color: "#00000000" }
            GradientStop { position: 0.5; color: "#BB000000" }
            GradientStop { position: 1.0; color: "#00000000" }
        }
    }

    UiLabel {
        anchors.left: altimeter.left;
        anchors.verticalCenter: altimeter.verticalCenter;
        anchors.horizontalCenter: altimeter.horizontalCenter;
        horizontalAlignment: Text.AlignHCenter;
        font.bold: true;
        text: Controllers.network.depth.toFixed(1);
        color: "white";
        style: Text.Outline;
        styleColor: Style.outlineColor;
    }
}
