import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtCharts 2.3

ColumnLayout {
    anchors.fill: parent;
    spacing: 0;
    property string tabTitle: "Charts";

    Repeater {
        model: 2;

        ChartView {
            id: chart;
            title: "Line";
            antialiasing: true;
            theme: ChartView.ChartThemeDark;
            Layout.fillWidth: true;
            Layout.fillHeight: true;

            LineSeries {
                id: ser;
                name: "LineSeries"
                property var selectedPoint: Qt.point(0, 0);

                XYPoint { x: 0; y: 0 }
                XYPoint { x: 1.1; y: 2.1 }
                XYPoint { x: 1.9; y: 3.3 }
                XYPoint { x: 2.1; y: 2.1 }
                XYPoint { x: 2.9; y: 4.9 }
                XYPoint { x: 3.4; y: 3.0 }
                XYPoint { x: 4.1; y: 3.3 }
            }

            Rectangle {
                id: r;
                color: "white";
                opacity: 0.5;
                width: 2;
                height: parent.height;
                x: chart.mapToPosition(ser.selectedPoint, ser).x;
            }

            MouseArea {
                anchors.fill: parent;
                hoverEnabled: true;

                onPositionChanged: {
                    ser.selectedPoint = chart.mapToValue(Qt.point(mouse.x, mouse.y), ser);
                }
            }
        }
    }
}

