import QtQuick 2.11
import QtQuick.Controls 2.12
import QtQml 2.2


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
                ctx.fillStyle = menuItem.highlighted ? "#9DA5B4" : Style.gray;
                ctx.moveTo(15, 15)
                ctx.lineTo(width - 15, height / 2)
                ctx.lineTo(15, height - 15)
                ctx.closePath()
                ctx.fill()
            }
        }

        contentItem: Text {
            leftPadding: menuItem.indicator.width
            rightPadding: menuItem.arrow.width
            text: menuItem.text
            font.pointSize: 10;
            opacity: enabled ? 1.0 : 0.3
            font.family: Style.fontSans;
            color: menuItem.highlighted ? "#9DA5B4" : Style.gray;
            elide: Text.ElideRight;
            renderType: TextEdit.NativeRendering;
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            implicitWidth: 20
            implicitHeight: 8
            opacity: enabled ? 1 : 0.3
            color: menuItem.highlighted ? Style.bgDarker : "transparent";
        }
    }

    background: Rectangle {
        implicitWidth: 180;
        implicitHeight: 30;
        color: Style.bgDark;

        Rectangle {
            color: Style.bgDarker;
            width: parent.width;
            height: 1;
            anchors.bottom: parent.bottom;
        }
    }

    PropertyAnimation on opacity {
        running: visible;
        from: 0.0;
        to: 1.0;
        duration: Style.animFast;
        easing.type: Style.animEasing;
    }
}
