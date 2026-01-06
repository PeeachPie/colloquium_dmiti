import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/*
 * Компонент для красивого отображения математических выражений
 */
Item {
    id: root
    
    property string expression: "0"
    property color textColor: "#FFFFFF"
    property int baseFontSize: 48
    property int minFontSize: 12
    
    property int effectiveFontSize: baseFontSize
    property bool needsScroll: false
    
    TextMetrics {
        id: textMetrics
        font.pixelSize: root.baseFontSize
        font.family: "SF Pro Display"
        text: root.expression
    }
    
    onExpressionChanged: recalculateFontSize()
    onWidthChanged: recalculateFontSize()
    onBaseFontSizeChanged: recalculateFontSize()
    
    Component.onCompleted: recalculateFontSize()
    
    function recalculateFontSize() {
        if (width <= 0 || expression.length === 0) {
            effectiveFontSize = baseFontSize
            needsScroll = false
            return
        }
        
        var size = baseFontSize
        var availableWidth = width - 16  // небольшой отступ
        
        var estimatedCharWidth = size * 0.55
        var textLength = expression.length
        
        while (size > minFontSize) {
            estimatedCharWidth = size * 0.55
            var estimatedWidth = textLength * estimatedCharWidth
            
            if (estimatedWidth <= availableWidth) {
                break
            }
            size -= 2
        }
        
        effectiveFontSize = Math.max(size, minFontSize)
        
        var finalEstimatedWidth = textLength * effectiveFontSize * 0.55
        needsScroll = (effectiveFontSize <= minFontSize && finalEstimatedWidth > availableWidth)
    }

    function parseExpression(expr) {
        var tokens = []
        var current = ""
        var i = 0
        
        while (i < expr.length) {
            var ch = expr[i]

            // дробь (число/число)
            var fractionMatch = expr.substring(i).match(/^(\d+)\/(\d+)/)
            if (fractionMatch) {
                if (current) {
                    tokens.push({ type: "text", value: current })
                    current = ""
                }
                tokens.push({ 
                    type: "fraction", 
                    numerator: fractionMatch[1], 
                    denominator: fractionMatch[2] 
                })
                i += fractionMatch[0].length
                continue
            }
            
            // степень (^число)
            var powerMatch = expr.substring(i).match(/^\^(\d+)/)
            if (powerMatch) {
                if (current) {
                    tokens.push({ type: "text", value: current })
                    current = ""
                }
                tokens.push({ type: "power", value: powerMatch[1] })
                i += powerMatch[0].length
                continue
            }
            
            // функции
            var funcMatch = expr.substring(i).match(/^(GCD\(|DER\(|FAC\(|NMR\()/)
            if (funcMatch) {
                if (current) {
                    tokens.push({ type: "text", value: current })
                    current = ""
                }
                
                var funcName = funcMatch[1]
                var openBracketPos = i + funcName.length - 1
                var bracketCount = 1
                var j = openBracketPos + 1
                
                while (j < expr.length && bracketCount > 0) {
                    if (expr[j] === '(') {
                        bracketCount++
                    } else if (expr[j] === ')') {
                        bracketCount--
                    }
                    j++
                }
                
                var foundClosingBracket = (bracketCount === 0)
                var closeBracketPos = j - 1
                var functionContent = expr.substring(openBracketPos + 1, foundClosingBracket ? closeBracketPos : expr.length)
                
                tokens.push({
                    type: "function", 
                    value: funcName
                })
                
                if (functionContent.length > 0) {
                    var contentTokens = parseExpression(functionContent)
                    tokens = tokens.concat(contentTokens)
                }
                
                if (foundClosingBracket) {
                    tokens.push({ 
                        type: "functionClose", 
                        value: ")"
                    })
                }
                
                i = foundClosingBracket ? (closeBracketPos + 1) : expr.length
                continue
            }
            
            if (ch === '(' || ch === ')') {
                if (current) {
                    tokens.push({ type: "text", value: current })
                    current = ""
                }
                tokens.push({ type: "text", value: ch })
                i++
                continue
            }
            
            current += ch
            i++
        }
        
        if (current) {
            tokens.push({ type: "text", value: current })
        }
        
        return tokens
    }
    
    ScrollView {
        id: scrollView
        anchors.fill: parent
        clip: true
        
        ScrollBar.horizontal.policy: needsScroll ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        
        contentWidth: mathFlow.width
        contentHeight: mathFlow.height
        
        Item {
            width: Math.max(mathFlow.width, scrollView.width)
            height: scrollView.height
            
            Flow {
                id: mathFlow
                anchors.centerIn: parent
                spacing: 4
                layoutDirection: Qt.LeftToRight
                
                Repeater {
                    model: parseExpression(root.expression)
                    
                    delegate: Loader {
                        sourceComponent: {
                            if (modelData.type === "fraction") {
                                return fractionComponent
                            } else if (modelData.type === "power") {
                                return powerComponent
                            } else if (modelData.type === "function") {
                                return functionComponent
                            } else if (modelData.type === "functionClose") {
                                return functionCloseComponent
                            } else {
                                return textComponent
                            }
                        }
                        
                        property var tokenData: modelData
                    }
                }
            }
        }
    }

    Component {
        id: textComponent
        
        Text {
            text: tokenData.value
            font.pixelSize: root.effectiveFontSize
            font.family: "SF Pro Display"
            color: root.textColor
            verticalAlignment: Text.AlignVCenter
            renderType: Text.NativeRendering
        }
    }

    // для дроби
    Component {
        id: fractionComponent
        
        Column {
            spacing: 2
            
            Text {
                id: numeratorText
                text: tokenData.numerator
                font.pixelSize: root.effectiveFontSize * 0.6
                font.family: "SF Pro Display"
                color: root.textColor
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
            
            Rectangle {
                width: Math.max(numeratorText.width, denominatorText.width) + 8
                height: 2
                color: root.textColor
                anchors.horizontalCenter: parent.horizontalCenter
            }
            
            Text {
                id: denominatorText
                text: tokenData.denominator
                font.pixelSize: root.effectiveFontSize * 0.6
                font.family: "SF Pro Display"
                color: root.textColor
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
    
    // для степени
    Component {
        id: powerComponent
        
        Text {
            text: {
                var superscripts = {'0':'⁰','1':'¹','2':'²','3':'³','4':'⁴','5':'⁵','6':'⁶','7':'⁷','8':'⁸','9':'⁹'}
                var result = ''
                for (var i = 0; i < tokenData.value.length; i++) {
                    result += superscripts[tokenData.value[i]] || tokenData.value[i]
                }
                return result
            }
            font.pixelSize: root.effectiveFontSize * 0.7
            font.family: "SF Pro Display"
            color: root.textColor
            verticalAlignment: Text.AlignTop
        }
    }
    
    // для функций
    Component {
        id: functionComponent
        
        Text {
            text: tokenData.value
            font.pixelSize: root.effectiveFontSize
            font.family: "SF Pro Display"
            color: "#007AFF"
            verticalAlignment: Text.AlignVCenter
            renderType: Text.NativeRendering
        }
    }
    
    Component {
        id: functionCloseComponent
        
        Text {
            text: tokenData.value
            font.pixelSize: root.effectiveFontSize
            font.family: "SF Pro Display"
            color: "#007AFF"
            verticalAlignment: Text.AlignVCenter
            renderType: Text.NativeRendering
        }
    }
}

