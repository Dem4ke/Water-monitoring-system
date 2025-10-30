import QtQuick 2.15
import QtQuick.Window 2.15
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: 1024
    height: 768

    property var coordinatesArray: [
        { latitude: -2.0, longitude: -9.4 },
        { latitude: -19.0, longitude: 5.0 },
        { latitude: -31.0, longitude: 14.0 }
    ]

    Plugin {
        id: mapPlugin
        name: "osm"  // OSM plugin supports custom tile servers

        // Example for a custom tile server, might not require an API key
                // Note: You need to find a suitable tile server and its URL format
                PluginParameter { name: "osm.mapping.custom.host"; value: "http://tile.openstreetmap.org/%z/%x/%y.png" }

    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
        center: QtPositioning.coordinate(37.7749, -122.4194)
        property var gpsCoord: QtPositioning.coordinate(-5, -5)
        zoomLevel: 2

        // GPS marker
        MapQuickItem {
            property int markerSize: 20

            id: gpsMarker
            coordinate: map.gpsCoord
            anchorPoint.x: markerSize/2
            anchorPoint.y: markerSize/2
            visible: posSource.active
            sourceItem: Rectangle {
                width: markerSize
                height: markerSize
                radius: markerSize/2
                border.color: "red"
                border.width: 2
                color: "red"
                Text {
                    anchors.centerIn: parent
                    text: "⦿"
                    font.pixelSize: markerSize
                    color: "red"
                }
            }
        }

        // Add red dot markers for each coordinate
        Repeater {
            model: coordinatesArray

            MapQuickItem {
                id: marker
                coordinate: QtPositioning.coordinate(modelData.latitude, modelData.longitude)
                anchorPoint.x: 6
                anchorPoint.y: 6
                sourceItem: Rectangle {
                    width: 12
                    height: 12
                    color: "red"
                    radius: 6
                    border.color: "black"
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        onClicked: popupMenu.open()
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
    }

    // Empty popup menu that appears when a marker is clicked
    Menu {
        id: popupMenu
        title: "Marker Menu"
        MenuItem { text: "Option 1" }
        MenuItem { text: "Option 2" }
        MenuItem { text: "Option 3" }
    }
}
