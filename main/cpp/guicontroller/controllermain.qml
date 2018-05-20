import QtLocation 5.3
import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.2
import QtQuick.Window 2.0
import io.bistromatics.backend 1.0

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("GuiController")

    Backend {
        id: backend
        objectName: 'backend'
        Component.onCompleted: {
            backend.Stepped.connect(stepped)
        }

        function stepped(infectedCount, day) {
            console.warn(infectedCount)
            console.warn(day)
            stepButton.enabled = true
            infectedNr.text = infectedCount
            dayNr.text = day
        }

    }

    ColumnLayout {
        RowLayout {
            Text {
                text: "Autostep: "
            }
            TextInput {
                id: secondsPerDay
                inputMethodHints: Qt.ImhDigitsOnly
                width: 20
                text: "2"
            }

            Text {
                text: "Seconds / Day"
            }
            Button {
                text: "Start"
                onClicked: {
                    if (text == "Start") {
                        stepTimer.interval = secondsPerDay.text * 1000
                        stepTimer.start()
                        text = "Stop"
                    } else {
                        stepTimer.stop()
                        text = "Start"
                    }

                }
            }
            Timer {
                id: stepTimer
                running: false
                repeat: true
                onTriggered: stepDay()
            }
        }
        RowLayout {
            Button {
                id: stepButton
                text: "Step day"
                onClicked: stepDay()
            }
        }

        RowLayout {
            Text {
                text: "Current day: "
            }
            Text {
                id: dayNr
                text: "0"
            }
        }
        RowLayout {
            Text {
                text: "Infected people: "
            }
            Text {
                id: infectedNr
                text: "0"
            }
        }
    }

    function stepDay() {
        stepButton.enabled = false
        backend.StepDay()
    }
}
