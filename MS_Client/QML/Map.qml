import QtQuick 2.15
import QtQuick.Window 2.15
import QtPositioning 5.15
import QtLocation 5.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: 1024
    height: 768

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
        //center: QtPositioning.coordinate(0, 0)
        zoomLevel: 2

        // GPS marker
        MapQuickItem {
            id: currentLocationMarker
            coordinate: mapData.currentLocation
            visible: mapData.currentLocation.isValid

            anchorPoint.x: sourceItem.width / 2
            anchorPoint.y: sourceItem.height / 2

            sourceItem: Rectangle {
                width: 16
                height: 16
                radius: 8
                color: "blue"
                border.color: "white"
                border.width: 2
            }
        }

        // Near vesels' locations
        MapItemView {
            model: mapData.locations

            delegate: MapQuickItem {
                id: locationMarker
                coordinate: modelData

                anchorPoint.x: sourceItem.width / 2
                anchorPoint.y: sourceItem.height / 2

                sourceItem: Rectangle {
                    width: 12
                    height: 12
                    radius: 6
                    color: "red"
                    border.color: "black"
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        onClicked: mapData.vesselDataRequested(index)
                    }
                }
            }
        }

        // Chosen vessel's road
        MapPolyline {
            id: roadLine
            line.width: 4
            line.color: "blue"
            path: mapData.roadMap
        }

        Component.onCompleted: {
            // Center map on current vessel coordinations
            if (mapData.currentLocation.isValid) {
                map.center = mapData.currentLocation
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
}
