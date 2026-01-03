import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/*
 * Вкладка калькулятора цепных дробей
 */
Item {
    id: cfTab
    
    property string numeratorInput: ""
    property string denominatorInput: ""
    property string cfInput: ""
    property string sqrtInput: ""
    property string maxDenInput: ""
    property int currentMode: 0  // 0: Q->CF, 1: CF->Q, 2: sqrt->CF, 3: CF->sqrt, 4: конвергенты, 5: приближение, 6: 1/x
    property int periodStart: -1  // Начало периода для режима CF->sqrt
    
    readonly property color backgroundColor: "#000000"
    readonly property color surfaceColor: "#1C1C1E"
    readonly property color primaryColor: "#007AFF"
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 140
            color: surfaceColor
            radius: 16
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 8
                
                Text {
                    id: historyLabel
                    Layout.fillWidth: true
                    text: "Цепные дроби"
                    font.pixelSize: 14
                    font.family: "SF Pro Display"
                    color: "#8E8E93"
                    horizontalAlignment: Text.AlignRight
                    renderType: Text.NativeRendering
                }
                
                // Для однострочного результата - MathDisplay
                MathDisplay {
                    id: mathResultDisplay
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    expression: ""
                    baseFontSize: 28
                    textColor: "#FFFFFF"
                    visible: !resultLabel.visible
                }
                
                // Для многострочного результата (конвергенты) - ScrollView
                ScrollView {
                    id: multiLineResult
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: false
                    clip: true
                    
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded
                    
                    Column {
                        id: resultColumn
                        width: multiLineResult.width
                        spacing: 4
                        
                        Repeater {
                            id: resultRepeater
                            model: []
                            
                            delegate: MathDisplay {
                                width: resultColumn.width
                                height: 32
                                expression: modelData
                                baseFontSize: 20
                                textColor: "#FFFFFF"
                            }
                        }
                    }
                }
                
                // Для текста-заглушки
                Text {
                    id: resultLabel
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: "Введите данные"
                    font.pixelSize: 28
                    font.family: "SF Pro Display"
                    font.weight: Font.Light
                    color: "#FFFFFF"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                    renderType: Text.NativeRendering
                    visible: true
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            spacing: 8
            
            Repeater {
                model: ["Q→CF", "CF→Q", "√D→CF", "CF→√D"]
                
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: currentMode === index ? primaryColor : surfaceColor
                    radius: 10
                    
                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: 13
                        font.family: "SF Pro Display"
                        font.weight: currentMode === index ? Font.Medium : Font.Normal
                        color: "#FFFFFF"
                        renderType: Text.NativeRendering
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            currentMode = index
                            clearInputs()
                        }
                    }
                }
            }
        }
        
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            spacing: 8
            
            Repeater {
                model: ["Конвергенты", "Приближение", "1/x"]
                
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: currentMode === (index + 4) ? primaryColor : surfaceColor
                    radius: 10
                    
                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: 13
                        font.family: "SF Pro Display"
                        font.weight: currentMode === (index + 4) ? Font.Medium : Font.Normal
                        color: "#FFFFFF"
                        renderType: Text.NativeRendering
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            currentMode = index + 4
                            clearInputs()
                        }
                    }
                }
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12
            visible: currentMode === 0
            
            Text {
                text: "Рациональное число (числитель/знаменатель)"
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }
            
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                
                TextField {
                    id: numField
                    Layout.fillWidth: true
                    placeholderText: "Числитель"
                    placeholderTextColor: "#636366"
                    text: numeratorInput
                    color: "#FFFFFF"
                    font.pixelSize: 20
                    font.family: "SF Pro Display"
                    horizontalAlignment: Text.AlignCenter
                    validator: RegularExpressionValidator { regularExpression: /^-?[0-9]*$/ }
                    background: Rectangle {
                        color: surfaceColor
                        radius: 10
                    }
                    onTextChanged: numeratorInput = text
                }
                
                Text {
                    text: "/"
                    font.pixelSize: 24
                    color: "#FFFFFF"
                }
                
                TextField {
                    id: denField
                    Layout.fillWidth: true
                    placeholderText: "Знаменатель"
                    placeholderTextColor: "#636366"
                    text: denominatorInput
                    color: "#FFFFFF"
                    font.pixelSize: 20
                    font.family: "SF Pro Display"
                    horizontalAlignment: Text.AlignCenter
                    validator: RegularExpressionValidator { regularExpression: /^-?[0-9]*$/ }
                    background: Rectangle {
                        color: surfaceColor
                        radius: 10
                    }
                    onTextChanged: denominatorInput = text
                }
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12
            visible: currentMode === 1 || currentMode === 4 || currentMode === 6
            
            Text {
                text: {
                    if (currentMode === 1) return "Цепная дробь [a₀; a₁, a₂, ...]"
                    if (currentMode === 4) return "Цепная дробь для конвергентов"
                    if (currentMode === 6) return "Цепная дробь для инвертирования"
                    return ""
                }
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }
            
            TextField {
                id: cfField
                Layout.fillWidth: true
                placeholderText: "Например: 3, 7, 15, 1"
                placeholderTextColor: "#636366"
                text: cfInput
                color: "#FFFFFF"
                font.pixelSize: 20
                font.family: "SF Pro Display"
                horizontalAlignment: Text.AlignCenter
                validator: RegularExpressionValidator { regularExpression: /^[-0-9,;\s]*$/ }
                background: Rectangle {
                    color: surfaceColor
                    radius: 10
                }
                onTextChanged: cfInput = text
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12
            visible: currentMode === 3
            
            Text {
                text: "Периодическая цепная дробь [a₀; a₁, ..., (b₁, b₂, ...)]"
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }
            
            TextField {
                id: cfFieldPeriodic
                Layout.fillWidth: true
                placeholderText: "Например: 1, 2 (для [1; (2)...])"
                placeholderTextColor: "#636366"
                text: cfInput
                color: "#FFFFFF"
                font.pixelSize: 20
                font.family: "SF Pro Display"
                horizontalAlignment: Text.AlignCenter
                validator: RegularExpressionValidator { regularExpression: /^[-0-9,;\s]*$/ }
                background: Rectangle {
                    color: surfaceColor
                    radius: 10
                }
                onTextChanged: cfInput = text
            }
            
            Text {
                text: "Индекс начала периода (0 = чисто периодическая)"
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }
            
            TextField {
                id: periodStartField
                Layout.fillWidth: true
                placeholderText: "Например: 1 (период с a₁)"
                placeholderTextColor: "#636366"
                text: periodStart >= 0 ? periodStart.toString() : ""
                color: "#FFFFFF"
                font.pixelSize: 20
                font.family: "SF Pro Display"
                horizontalAlignment: Text.AlignCenter
                validator: RegularExpressionValidator { regularExpression: /^[0-9]*$/ }
                background: Rectangle {
                    color: surfaceColor
                    radius: 10
                }
                onTextChanged: periodStart = text === "" ? -1 : parseInt(text)
            }
        }
        
        // Поля для режима приближения
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12
            visible: currentMode === 5
            
            Text {
                text: "Цепная дробь [a₀; a₁, a₂, ...]"
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }
            
            TextField {
                id: cfFieldApprox
                Layout.fillWidth: true
                placeholderText: "Например: 3, 7, 15, 1, 292"
                placeholderTextColor: "#636366"
                text: cfInput
                color: "#FFFFFF"
                font.pixelSize: 20
                font.family: "SF Pro Display"
                horizontalAlignment: Text.AlignCenter
                validator: RegularExpressionValidator { regularExpression: /^[-0-9,;\s]*$/ }
                background: Rectangle {
                    color: surfaceColor
                    radius: 10
                }
                onTextChanged: cfInput = text
            }
            
            Text {
                text: "Максимальный знаменатель"
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }
            
            TextField {
                id: maxDenField
                Layout.fillWidth: true
                placeholderText: "Например: 100"
                placeholderTextColor: "#636366"
                text: maxDenInput
                color: "#FFFFFF"
                font.pixelSize: 20
                font.family: "SF Pro Display"
                horizontalAlignment: Text.AlignCenter
                validator: RegularExpressionValidator { regularExpression: /^[0-9]*$/ }
                background: Rectangle {
                    color: surfaceColor
                    radius: 10
                }
                onTextChanged: maxDenInput = text
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12
            visible: currentMode === 2
            
            Text {
                text: "Квадратный корень √D"
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }
            
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                
                Text {
                    text: "√"
                    font.pixelSize: 28
                    color: "#FFFFFF"
                }
                
                TextField {
                    id: sqrtField
                    Layout.fillWidth: true
                    placeholderText: "D (например: 2)"
                    placeholderTextColor: "#636366"
                    text: sqrtInput
                    color: "#FFFFFF"
                    font.pixelSize: 20
                    font.family: "SF Pro Display"
                    horizontalAlignment: Text.AlignCenter
                    validator: RegularExpressionValidator { regularExpression: /^[0-9]*$/ }
                    background: Rectangle {
                        color: surfaceColor
                        radius: 10
                    }
                    onTextChanged: sqrtInput = text
                }
            }
        }
        
        Item { Layout.fillHeight: true }
        
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            spacing: 12
            
            Rectangle {
                Layout.preferredWidth: 120
                Layout.fillHeight: true
                color: surfaceColor
                radius: 12
                
                Text {
                    anchors.centerIn: parent
                    text: "Очистить"
                    font.pixelSize: 18
                    font.family: "SF Pro Display"
                    color: "#FF3B30"
                    renderType: Text.NativeRendering
                }
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: clearInputs()
                }
            }
            
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: primaryColor
                radius: 12
                
                Text {
                    anchors.centerIn: parent
                    text: "Вычислить"
                    font.pixelSize: 18
                    font.family: "SF Pro Display"
                    font.weight: Font.Medium
                    color: "#FFFFFF"
                    renderType: Text.NativeRendering
                }
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: calculate()
                }
            }
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "transparent"
            
            Text {
                anchors.centerIn: parent
                text: getModeDescription()
                font.pixelSize: 12
                font.family: "SF Pro Display"
                color: "#8E8E93"
                renderType: Text.NativeRendering
            }
        }
    }
    
    function getModeDescription() {
        switch(currentMode) {
            case 0: return "Преобразование рационального числа в цепную дробь"
            case 1: return "Преобразование цепной дроби в рациональное число"
            case 2: return "Цепная дробь для квадратного корня"
            case 3: return "Периодическая ЦД в квадратичную иррациональность"
            case 4: return "Вычисление подходящих дробей (конвергентов)"
            case 5: return "Приближение с ограничением знаменателя"
            case 6: return "Инвертирование цепной дроби (1/x)"
            default: return ""
        }
    }
    
    function clearInputs() {
        numeratorInput = ""
        denominatorInput = ""
        cfInput = ""
        sqrtInput = ""
        maxDenInput = ""
        periodStart = -1
        numField.text = ""
        denField.text = ""
        cfField.text = ""
        sqrtField.text = ""
        maxDenField.text = ""
        cfFieldApprox.text = ""
        cfFieldPeriodic.text = ""
        periodStartField.text = ""
        
        // Сброс отображения
        resultLabel.text = "Введите данные"
        resultLabel.visible = true
        mathResultDisplay.expression = ""
        mathResultDisplay.visible = false
        multiLineResult.visible = false
        resultRepeater.model = []
        
        historyLabel.text = "Цепные дроби"
    }
    
    function showSingleResult(text) {
        resultLabel.visible = false
        multiLineResult.visible = false
        mathResultDisplay.expression = text
        mathResultDisplay.visible = true
    }
    
    function showMultiLineResult(lines) {
        resultLabel.visible = false
        mathResultDisplay.visible = false
        resultRepeater.model = lines
        multiLineResult.visible = true
    }
    
    function showError(text) {
        mathResultDisplay.visible = false
        multiLineResult.visible = false
        resultLabel.text = text
        resultLabel.visible = true
    }
    
    function calculate() {
        var result = ""
        
        try {
            switch(currentMode) {
                case 0: // Q -> CF
                    if (numeratorInput === "" || denominatorInput === "") {
                        showError("Введите числитель и знаменатель")
                        return
                    }
                    result = calculatorBackend.rationalToCF(numeratorInput, denominatorInput)
                    historyLabel.text = numeratorInput + "/" + denominatorInput + " ="
                    break
                    
                case 1: // CF -> Q
                    if (cfInput === "") {
                        showError("Введите коэффициенты")
                        return
                    }
                    result = calculatorBackend.cfToRational(cfInput)
                    historyLabel.text = "[" + cfInput + "] ="
                    break
                    
                case 2: // sqrt -> CF
                    if (sqrtInput === "") {
                        showError("Введите D")
                        return
                    }
                    result = calculatorBackend.sqrtToCF(sqrtInput)
                    historyLabel.text = "√" + sqrtInput + " ="
                    break
                    
                case 3: // CF -> sqrt (периодическая ЦД в квадратичную иррациональность)
                    if (cfInput === "") {
                        showError("Введите коэффициенты")
                        return
                    }
                    if (periodStart < 0) {
                        showError("Укажите начало периода")
                        return
                    }
                    result = calculatorBackend.cfToQuad(cfInput, periodStart)
                    historyLabel.text = "[" + cfInput + "] ="
                    break
                    
                case 4: // Конвергенты
                    if (cfInput === "") {
                        showError("Введите коэффициенты")
                        return
                    }
                    result = calculatorBackend.cfConvergents(cfInput)
                    historyLabel.text = "Конвергенты [" + cfInput + "]:"
                    break
                    
                case 5: // Приближение
                    if (cfInput === "" || maxDenInput === "") {
                        showError("Введите ЦД и макс. знаменатель")
                        return
                    }
                    result = calculatorBackend.cfApprox(cfInput, maxDenInput)
                    historyLabel.text = "Прибл. [" + cfInput + "], q≤" + maxDenInput + " ="
                    break
                    
                case 6: // 1/x
                    if (cfInput === "") {
                        showError("Введите коэффициенты")
                        return
                    }
                    result = calculatorBackend.cfInvert(cfInput)
                    historyLabel.text = "1/[" + cfInput + "] ="
                    break
            }
            
            if (result.startsWith("Ошибка")) {
                showError("Ошибка")
            } else if (currentMode === 4) {
                // Конвергенты - многострочный вывод
                var lines = result.split("\n")
                showMultiLineResult(lines)
            } else {
                showSingleResult(result)
            }
            
        } catch (error) {
            showError("Ошибка")
        }
    }
}
