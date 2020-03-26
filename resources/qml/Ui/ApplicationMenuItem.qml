import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2
import QtQuick.Controls.Styles 1.4

Menu {
    id: menu

    delegate: MenuItem {
        id: menuItem
        implicitWidth: 100
        implicitHeight: 30

        arrow: Canvas {
            x: parent.width - width
            y: parent.height - height + 5
            implicitWidth: 40
            implicitHeight: 40
            visible: menuItem.subMenu
            onPaint: {
                var ctx = getContext("2d")
                ctx.fillStyle = menuItem.highlighted ? "#9DA5B4" : "#6E7582";
                ctx.moveTo(15, 15)
                ctx.lineTo(width - 15, height / 2)
                ctx.lineTo(15, height - 15)
                ctx.closePath()
                ctx.fill()
            }
        }

        indicator: Item {
            implicitWidth: 30
            implicitHeight: 30
            Rectangle {
                width: 15
                height: 15
                anchors.centerIn: parent
                visible: menuItem.checkable
                border.color: "#181A1F"
                color: "#6E7582";
                radius: 3
                Rectangle {
                    width: 9
                    height: 9
                    anchors.centerIn: parent
                    visible: menuItem.checked
                    color: "#181A1F"
                    radius: 2
                }
            }
        }

        contentItem: Text {
            leftPadding: menuItem.indicator.width
            rightPadding: menuItem.arrow.width
            text: menuItem.text
            font.pointSize: 10;
            opacity: enabled ? 1.0 : 0.3
            font.family: "Segoe WPC";
            color: menuItem.highlighted ? "#9DA5B4" : "#6E7582";
            elide: Text.ElideRight;
            renderType: TextEdit.NativeRendering;
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            implicitWidth: 20
            implicitHeight: 8
            opacity: enabled ? 1 : 0.3
            color: menuItem.highlighted ? "#181A1F" : "transparent";
        }
    }

    background: Rectangle {
        implicitWidth: 180;
        implicitHeight: 30;
        color: "#21252B";

        Rectangle {
            color: "#181A1F";
            width: parent.width;
            height: 1;
            anchors.bottom: parent.bottom;
        }
    }
}
