import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/*
 * Компонент дисплея калькулятора
 * Показывает текущее выражение и историю вычислений
 */
Rectangle {
    id: root

    property string mainText: "0"
    property string historyText: ""
    property bool showHistory: true
    
    color: "#1C1C1E"
    radius: 16
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 8
        
        ScrollView {
            id: historyView
            Layout.fillWidth: true
            Layout.preferredHeight: showHistory ? 80 : 0
            visible: showHistory && historyText !== ""
            clip: true
            
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            
            background: Rectangle {
                color: "transparent"
            }
            
            Text {
                id: history
                width: historyView.width
                text: historyText
                font.pixelSize: 16
                font.family: "SF Mono"
                color: "#8E8E93"
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignRight
                renderType: Text.NativeRendering
            }
            
            Behavior on Layout.preferredHeight {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#2C2C2E"
            visible: showHistory && historyText !== ""
        }
        
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 100
            
            MathDisplay {
                id: mainDisplay
                anchors.fill: parent
                anchors.margins: 5
                
                expression: mainText
                baseFontSize: {
                    if (mainText.length > 50) return 24
                    if (mainText.length > 30) return 32
                    if (mainText.length > 15) return 40
                    return 48
                }
                textColor: "#FFFFFF"
            }
        }
        
        Rectangle {
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: 2
            Layout.preferredHeight: 40
            color: "#007AFF"
            opacity: 0.7
            visible: false
        }
    }
    
    SequentialAnimation {
        id: updateAnimation
        
        PropertyAnimation {
            target: root
            property: "border.color"
            to: "#007AFF"
            duration: 100
        }
        PropertyAnimation {
            target: root
            property: "border.width"
            to: 2
            duration: 100
        }
        PauseAnimation { duration: 200 }
        PropertyAnimation {
            target: root
            property: "border.width"
            to: 0
            duration: 200
        }
    }
    
    function triggerUpdate() {
        updateAnimation.restart()
    }
    
    function appendText(text) {
        if (mainText === "0") {
            mainText = text
        } else {
            mainText += text
        }
        triggerUpdate()
    }
    
    function clear() {
        mainText = "0"
        historyText = ""
    }
    
    function backspace() {
        if (mainText.length > 1) {
            mainText = mainText.slice(0, -1)
        } else {
            mainText = "0"
        }
    }
}

