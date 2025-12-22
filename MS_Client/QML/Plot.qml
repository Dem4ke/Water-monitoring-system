import QtQuick 2.15
import QtCharts 2.15

ChartView {
    anchors.fill: parent
    antialiasing: true

    DateTimeAxis {
        id: axisX
        min: new Date(plotData.minTime)
        max: new Date(plotData.maxTime)
        format: "hh:mm:ss"
        titleText: "Time"
    }

    ValueAxis {
        id: axisY
        min: plotData.minValue
        max: plotData.maxValue
        titleText: "Value"
    }

    LineSeries {
        id: series
        name: plotData.name
        axisX: axisX
        axisY: axisY
    }

    Connections {
        target: plotData

        function onRedrawPlotRequest() {
            series.clear()

            const pts = plotData.points
            for (let i = 0; i < pts.length; ++i) {
                series.append(pts[i].x, pts[i].y)
            }
        }
    }
}
