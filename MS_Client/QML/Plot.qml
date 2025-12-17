import QtQuick
import QtCharts

ChartView {
    width: 400
    height: 300
    title: "Simple Line Plot"
    antialiasing: true

    ValueAxis { id: axisX; min: 0; max: 3 }
    ValueAxis { id: axisY; min: 0; max: 3 }

    LineSeries {
        axisX: axisX
        axisY: axisY
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1; y: 2 }
        XYPoint { x: 2; y: 1 }
        XYPoint { x: 3; y: 3 }
    }
}
