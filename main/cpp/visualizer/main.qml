import QtQuick 2.0
import QtQuick.Controls 1.3
import QtLocation 5.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0
import QtPositioning 5.5
import io.bistromatics.backend 1.0
import "components"
import "models"

ApplicationWindow {
	id: window
	visible: true
	width: 640
	height: 480
	title: qsTr("Visualizer")

	RowLayout {
		spacing: 6
		anchors.fill: parent
        anchors.margins: 20
        Layout.fillWidth: true

        // LEFT COLUMN
        LocationViewer {
            id: locViewer
        }

        // MIDDLE COLUMN Contact Center info
        ContactCenterViewer {}

		ColumnLayout {
			Layout.rightMargin: 4
            Layout.fillWidth: false
			CheckBox {
				id: checkBox_School
				text: qsTr("School")
				checked: true
			}

			CheckBox {
				id: checkBox_HighSchool
				text: qsTr("HighSchool")
				checked: true
			}

			CheckBox {
				id: checkBox_Workplace
				text: qsTr("Workplace")
				checked: true
			}

			CheckBox {
				id: checkBox_HouseHold
				text: qsTr("HouseHold")
				checked: true
			}
			CheckBox {
				id: checkBox_Commutes
				text: qsTr("Commutes")
				checked: true
			}

			Button {
				id: button
				text: qsTr("Open File")
                onClicked: fileSelector.open()
				checked: true
			}


		}
    }

    Window {
        width: 512
        height: 512
        visible: true

        Plugin {
            id: mapPlugin
            name: "osm"
			PluginParameter { name: "osm.mapping.host"; value: "https://tile.openstreetmap.org/" }
			PluginParameter { name: "osm.geocoding.host"; value: "https://nominatim.openstreetmap.org" }
			PluginParameter { name: "osm.routing.host"; value: "https://router.project-osrm.org/viaroute" }
			PluginParameter { name: "osm.places.host"; value: "https://nominatim.openstreetmap.org/search" }
			PluginParameter { name: "osm.mapping.copyright"; value: "" }
			PluginParameter { name: "osm.mapping.highdpi_tiles"; value: true }
        }

        Map {
            id: map
            anchors.fill: parent
            plugin: mapPlugin
            zoomLevel: 14
            center: QtPositioning.coordinate(51.2, 4.4)

			Component.onCompleted: {
				backend.setObjects(map)
				for( var i_type in supportedMapTypes  ) {
					if( supportedMapTypes[i_type].name.localeCompare( "Custom URL Map"  ) === 0  ) {
                    activeMapType = supportedMapTypes[i_type]
                
					}
				}
			}

            function addMarker(lon, lat, markerID) {
                console.warn("qml ADDING MARKER" + lon +  lat + markerID)
                var marker = Qt.createQmlObject("   import QtLocation 5.3;
                                                    import QtQuick 2.7;
                                                    MapQuickItem {
                                                        id: marker
                                                        sourceItem: Rectangle { width: 14; height: 14; color: '#e41e25'; border.width: 2; border.color: 'white'; smooth: true; radius: 7 }
                                                       coordinate {
                                                                   latitude: 51.2
                                                                   longitude: 4.4
                                                               }
                                                        opacity: 1.0
                                                        property var markerID: ''

                                                        function setID(newID) {
                                                            markerID = newID
                                                        }
                                                        signal clicked(string id)
                                                        MouseArea {
                                                            anchors.fill: parent
                                                            onClicked: {parent.clicked(markerID)}
                                                        }

                                                    }", map)
                marker.clicked.connect(markerClicked)
                marker.setID(markerID)
                marker.coordinate.latitude = lat
                marker.coordinate.longitude = lon
                map.addMapItem(marker)
            }

            function markerClicked(id) {
                backend.onMarkerClicked(id)
            }
        }
    }

    BackEnd {
        id: backend
        Component.onCompleted: {
            backend.locationSelected.connect(clickSignal)
        }

        function clickSignal (arg) {
            locViewer.showLocation(arg)
        }
    }

    FileDialog {
        id: fileSelector
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + fileSelector.fileUrls)
            backend.loadGeoGridFromFile(fileSelector.fileUrl, map)
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
    }
}
