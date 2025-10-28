import QtQuick 2.15
import QtQuick.Controls 2.15
import QtPositioning 5.15
import QtLocation 5.15

Item {
    id: root
    width: 1024
    height: 768

    property bool showLabels: true
    property bool followGps: true
    property real markerSize: 18

    Plugin {
        id: stadiaPlugin
        name: "osm"

        PluginParameter {
            name: "osm.mapping.custom.url"
            value: "https://tiles.stadiamaps.com/tiles/alidade_smooth/{z}/{x}/{y}.png?api_key=91a69f1e-7415-4963-bbec-497a7b2e3d53"
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: stadiaPlugin
        zoomLevel: 4
        center: QtPositioning.coordinate(0, 0)
        activeMapType: supportedMapTypes[0]

        property int gridInterval: 10          // degrees between grid lines (set to 1,5,10,...)

        // -- GPS tracker: PositionSource
        PositionSource {
            id: posSource
            active: true
            updateInterval: 1000          // ms
            preferredPositioningMethods: PositionSource.AllPositioningMethods
            onPositionChanged: {
                gpsCoord = position.coordinate
                gpsAccuracy = position.attribute(PositionSource.SatellitePositioningAccuracy) !== undefined ?
                              position.attribute(PositionSource.SatellitePositioningAccuracy) : position.coordinateAccuracy
                if (followGps) {
                    // smoothly center on GPS
                    map.center = gpsCoord
                }
            }
        }

        // store last known coords/accuracy
        property var gpsCoord: QtPositioning.coordinate(0, 0)
        property real gpsAccuracy: 0

        // Accuracy circle
        MapCircle {
            center: map.gpsCoord
            radius: map.gpsAccuracy > 0 ? map.gpsAccuracy : 0
            visible: posSource.active && gpsAccuracy > 0
            border.width: 1
            border.color: "steelblue"
            opacity: 0.25
        }

        // GPS marker
        MapQuickItem {
            id: gpsMarker
            coordinate: map.gpsCoord
            anchorPoint.x: markerSize/2
            anchorPoint.y: markerSize/2
            visible: posSource.active
            sourceItem: Rectangle {
                width: markerSize
                height: markerSize
                radius: markerSize/2
                border.color: "white"
                border.width: 2
                color: "red"
                Text {
                    anchors.centerIn: parent
                    text: "⦿"
                    font.pixelSize: markerSize * 0.6
                    color: "white"
                }
            }
        }

        ListModel { id: latLinesModel }
        ListModel { id: lonLinesModel }

        // --- Grid generation (latitudinal and longitudinal lines)
        // We'll create a model array of latitudes and longitudes based on gridInterval.
        function buildGridLines() {
            latLinesModel.clear()
            lonLinesModel.clear()

            if (gridInterval <= 0) return

            // latitudes: -90 .. 90
            for (var lat = -90; lat <= 90; lat += gridInterval) {
                latLinesModel.append({"latitude": lat})
            }
            // longitudes: -180 .. 180
            for (var lon = -180; lon <= 180; lon += gridInterval) {
                lonLinesModel.append({"longitude": lon})
            }
        }

        Component.onCompleted: buildGridLines()
        onGridIntervalChanged: buildGridLines()



        // For each latitude line we build a MapPolyline going from longitude -180 to 180 with small steps
        Repeater {
            model: latLinesModel
            delegate: MapPolyline {
                path: {
                    var pts = []
                    var lat = model.latitude
                    for (var lon = -180; lon <= 180; lon += 1)
                        pts.push(QtPositioning.coordinate(lat, lon))
                    return pts
                }
                line.width: 10
                line.color: "red"
                opacity: 0.5
            }
        }

        // For each longitude line, build a MapPolyline from lat -90 to 90
        Repeater {
            model: lonLinesModel
            delegate: MapPolyline {
                path: {
                    var pts = []
                    var lon = model.longitude
                    for (var lat = -90; lat <= 90; lat += 1)
                        pts.push(QtPositioning.coordinate(lat, lon))
                    return pts
                }
                line.width: 10
                line.color: "red"
                opacity: 0.5
            }
        }

        // Labels (latitude numbers at right edge and longitude numbers at top)
        // We'll place labels every gridInterval degrees along map edges or at map center projection fallback.
        Repeater {
            model: latLinesModel
            delegate: MapQuickItem {
                coordinate: {
                    // Make sure visibleRegion and boundingRectangle exist first
                    if (map.visibleRegion && map.visibleRegion.boundingRectangle) {
                        return QtPositioning.coordinate(
                            model.latitude,
                            map.visibleRegion.boundingRectangle.topRight.longitude
                        )
                    } else {
                        // fallback to center longitude if region not ready yet
                        return QtPositioning.coordinate(model.latitude, map.center.longitude)
                    }
                }

                anchorPoint.x: 0
                anchorPoint.y: 8
                visible: showLabels

                sourceItem: Rectangle {
                    color: "#ffffffcc"
                    radius: 2
                    border.color: "#ccc"
                    anchors.centerIn: parent
                    anchors.margins: 2

                    Text {
                        anchors.centerIn: parent
                        text: model.latitude + "°"
                        font.pixelSize: 12
                        color: "black"
                    }
                }
            }
        }

        Repeater {
            model: lonLinesModel
            delegate: MapQuickItem {
                coordinate: {
                    // Safely compute coordinate only when visibleRegion is ready
                    if (map.visibleRegion && map.visibleRegion.boundingRectangle) {
                        return QtPositioning.coordinate(
                            map.visibleRegion.boundingRectangle.bottomLeft.latitude,
                            model.longitude
                        )
                    } else {
                        // Fallback to center latitude until map is ready
                        return QtPositioning.coordinate(map.center.latitude, model.longitude)
                    }
                }

                anchorPoint.x: 12
                anchorPoint.y: 0
                visible: showLabels && map.visibleRegion && map.visibleRegion.boundingRectangle

                sourceItem: Rectangle {
                    color: "#ffffffcc"
                    radius: 2
                    border.color: "#ccc"
                    anchors.centerIn: parent
                    anchors.margins: 2

                    Text {
                        anchors.centerIn: parent
                        text: (model.longitude !== undefined ? model.longitude : 0) + "°"
                        font.pixelSize: 12
                        color: "black"
                    }
                }
            }
        }

        // Zoom with mouse wheel
        WheelHandler {
            id: wheel
            // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
            // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
            // and we don't yet distinguish mice and trackpads on Wayland either
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                             ? PointerDevice.Mouse | PointerDevice.TouchPad
                             : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }

        // Pan with right mouse drag
        DragHandler {
            id: drag
            target: null
            acceptedButtons: Qt.RightButton
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }

    } // Map

    // --- UI Controls
    Column {
        anchors {
            right: parent.right
            top: parent.top
            margins: 12
        }
        spacing: 8

        Rectangle {
            width: 220; height: 36
            radius: 6; color: "#ffffffcc"; border.color: "#888"
            Row { anchors.fill: parent; anchors.margins: 6; spacing: 6
                CheckBox { id: followChk; checked: followGps; onCheckedChanged: followGps = checked }
                Label { text: "Follow GPS"; anchors.verticalCenter: parent.verticalCenter }
            }
        }

        Rectangle {
            width: 220; height: 36
            radius: 6; color: "#ffffffcc"; border.color: "#888"
            Row { anchors.fill: parent; anchors.margins: 6; spacing: 6
                Label { text: "Grid interval (°):" }
                SpinBox {
                    from: 1; to: 30; value: root.gridInterval
                    onValueChanged: root.gridInterval = value
                }
            }
        }

        Rectangle {
            width: 220; height: 36
            radius: 6; color: "#ffffffcc"; border.color: "#888"
            Row { anchors.fill: parent; anchors.margins: 6; spacing: 6
                CheckBox { checked: root.showLabels; onCheckedChanged: root.showLabels = checked }
                Label { text: "Show labels" }
            }
        }

        Rectangle {
            width: 220; height: 36
            radius: 6; color: "#ffffffcc"; border.color: "#888"
            Row { anchors.fill: parent; anchors.margins: 6; spacing: 6
                Button {
                    text: "Center to (0,0)"
                    onClicked: map.center = QtPositioning.coordinate(0, 0)
                }
                Button {
                    text: "Zoom to world"
                    onClicked: map.zoomLevel = 2
                }
            }
        }
    }
}
