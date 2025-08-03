import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Rectangle {
    width: parent.width
    height: parent.height
    color: "#e8e8e8"

    // 地图插件配置
    Plugin {
        id: mapPlugin
        name: "osm"
        PluginParameter { name: "osm.mapping.custom.host"; value: "http://tile.openstreetmap.org" }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(39.9042, 116.4074)  // 北京坐标
        zoomLevel: 12

        // 启用手势
        gesture.enabled: true
        gesture.flickDeceleration: 3000

        // 添加比例尺
        MapScale {
            id: scale
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: 10
        }

        // 添加指南针
        MapCompass {
            id: compass
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 10
            map: map
        }
         onCenterChanged: {
        console.log("中心坐标:", center.latitude, center.longitude)
    }
    
    // 添加点击标记功能
    MouseArea {
        anchors.fill: parent
        onClicked: {
            var coordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
            addMarker(coordinate)
        }
    }
    
    function addMarker(coordinate) {
        var component = Qt.createComponent("Marker.qml")
        if (component.status === Component.Ready) {
            var marker = component.createObject(map, {coordinate: coordinate})
        }
    }
    
}
