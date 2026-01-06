import QtQuick
import QtQuick.Controls

/*
 * Кнопка копирования
 */
Item {
    id: root
    width: 24
    height: 24
    
    property string textToCopy: ""
    property color iconColor: "#8E8E93"
    property color hoverColor: "#FFFFFF"
    
    // false = иконка копирования, true = галочка
    property bool copied: false
    
    function drawRoundedRect(ctx, x, y, w, h, r) {
        ctx.beginPath()
        ctx.moveTo(x + r, y)
        ctx.lineTo(x + w - r, y)
        ctx.arcTo(x + w, y, x + w, y + r, r)
        ctx.lineTo(x + w, y + h - r)
        ctx.arcTo(x + w, y + h, x + w - r, y + h, r)
        ctx.lineTo(x + r, y + h)
        ctx.arcTo(x, y + h, x, y + h - r, r)
        ctx.lineTo(x, y + r)
        ctx.arcTo(x, y, x + r, y, r)
        ctx.closePath()
    }
    
    Canvas {
        id: canvas
        anchors.fill: parent
        
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            
            var color = mouseArea.containsMouse ? root.hoverColor : root.iconColor
            ctx.strokeStyle = color
            ctx.lineWidth = 1.5
            ctx.lineCap = "round"
            ctx.lineJoin = "round"
            
            if (root.copied) {
                // галочка
                ctx.beginPath()
                ctx.moveTo(5, 12)
                ctx.lineTo(10, 17)
                ctx.lineTo(19, 7)
                ctx.stroke()
            } else {
                var r = 2
                // задний прямоугольник (правый верхний)
                root.drawRoundedRect(ctx, 8, 2, 13, 14, r)
                ctx.stroke()
                // передний прямоугольник (левый нижний) - с заливкой фона
                ctx.fillStyle = "#1C1C1E"
                root.drawRoundedRect(ctx, 3, 6, 13, 14, r)
                ctx.fill()
                ctx.stroke()
            }
        }
    }
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        
        onClicked: {
            if (root.textToCopy !== "") {
                clipboardHelper.text = root.textToCopy
                clipboardHelper.selectAll()
                clipboardHelper.copy()

                root.copied = true
                canvas.requestPaint()
                
                // через 1.5 секунды возвращаем иконку копирования
                resetTimer.start()
            }
        }
        
        onContainsMouseChanged: canvas.requestPaint()
    }

    TextEdit {
        id: clipboardHelper
        visible: false
    }
    
    Timer {
        id: resetTimer
        interval: 1500
        onTriggered: {
            root.copied = false
            canvas.requestPaint()
        }
    }
}
