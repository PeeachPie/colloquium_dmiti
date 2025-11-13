import QtQuick
import QtQuick.Effects

/*
 * Компонент кнопки калькулятора
 */
Item {
    id: root
    
    property string buttonText: ""
    property string buttonType: "default"  // default, operator, special, equals
    property color buttonColor: getButtonColor()
    property color textColor: getTextColor()
    property real fontSize: 20
    property bool enableHoverEffect: true
    property real textVerticalOffset: 0
    
    property bool pressed: false
    property bool hovered: false
    
    implicitWidth: 70
    implicitHeight: 70
    
    signal clicked()
    
    function getButtonColor() {
        switch(buttonType) {
            case "operator":
                return "#FF9500"
            case "special":
                return "#505050"
            case "equals":
                return "#34C759"
            case "clear":
                return "#FF3B30"
            case "function":
                return "#007AFF"
            default:
                return "#333333"
        }
    }
    
    function getTextColor() {
        return "#FFFFFF"
    }
    
    Rectangle {
        id: buttonBackground
        anchors.fill: parent
        color: root.pressed ? Qt.darker(buttonColor, 1.3) : 
               (root.hovered && enableHoverEffect ? Qt.lighter(buttonColor, 1.2) : buttonColor)
        radius: 12
        
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowColor: "#40000000"
            shadowBlur: 0.4
            shadowVerticalOffset: 3
            shadowHorizontalOffset: 0
        }
        
        transform: Scale {
            origin.x: buttonBackground.width / 2
            origin.y: buttonBackground.height / 2
            xScale: root.pressed ? 0.95 : 1.0
            yScale: root.pressed ? 0.95 : 1.0
            
            Behavior on xScale { NumberAnimation { duration: 100 } }
            Behavior on yScale { NumberAnimation { duration: 100 } }
        }
        
        Behavior on color {
            ColorAnimation { duration: 150 }
        }
    }
    
    Text {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: textVerticalOffset
        text: buttonText
        font.pixelSize: fontSize
        font.family: "SF Pro Display"
        font.weight: Font.Medium
        color: textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
    }
    
    Rectangle {
        id: ripple
        anchors.centerIn: parent
        width: 0
        height: width
        radius: width / 2
        color: "#40FFFFFF"
        opacity: 0
        
        ParallelAnimation {
            id: rippleAnimation
            NumberAnimation {
                target: ripple
                property: "width"
                from: 0
                to: root.width * 2
                duration: 400
                easing.type: Easing.OutQuad
            }
            NumberAnimation {
                target: ripple
                property: "opacity"
                from: 0.6
                to: 0
                duration: 400
                easing.type: Easing.OutQuad
            }
        }
    }
    
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        
        onPressed: {
            root.pressed = true
            rippleAnimation.restart()
        }
        
        onReleased: {
            root.pressed = false
        }
        
        onClicked: {
            root.clicked()
        }
        
        onEntered: {
            root.hovered = true
        }
        
        onExited: {
            root.hovered = false
            root.pressed = false
        }
    }
}

