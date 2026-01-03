import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/*
 * Вкладка калькулятора полиномиальных цепных дробей
 */
Item {
    id: pcfTab

    property string numeratorPoly: ""
    property string denominatorPoly: ""
    property string pcfInput: ""
    property int currentMode: 0  // 0: P/Q->PCF, 1: PCF->P/Q, 2: конвергенты, 3: 1/x

    readonly property color backgroundColor: "#000000"
    readonly property color surfaceColor: "#1C1C1E"
    readonly property color primaryColor: "#007AFF"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        // Результат
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
                    text: "Полиномиальные цепные дроби"
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
                    baseFontSize: 22
                    textColor: "#FFFFFF"
                    visible: false
                }
                
                // Для многострочного результата - ScrollView
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
                                height: 28
                                expression: modelData
                                baseFontSize: 18
                                textColor: "#FFFFFF"
                            }
                        }
                    }
                }

                // Для текста-заглушки и ошибок
                Text {
                    id: resultLabel
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: "Введите данные"
                    font.pixelSize: 22
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

        // Выбор режима
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            spacing: 8

            Repeater {
                model: ["P/Q→PCF", "PCF→P/Q", "Конверг.", "1/x"]

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: currentMode === index ? primaryColor : surfaceColor
                    radius: 10

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: 14
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

        // Поля ввода для P/Q -> PCF
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12
            visible: currentMode === 0

            Text {
                text: "Рациональная функция P(x) / Q(x)"
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }

            TextField {
                id: numPolyField
                Layout.fillWidth: true
                placeholderText: "P(x), например: x^3 + 2x + 1"
                placeholderTextColor: "#636366"
                text: numeratorPoly
                color: "#FFFFFF"
                font.pixelSize: 18
                font.family: "SF Pro Display"
                validator: RegularExpressionValidator { regularExpression: /^[-0-9xX^+*/()\s]*$/ }
                background: Rectangle {
                    color: surfaceColor
                    radius: 10
                }
                onTextChanged: numeratorPoly = text
            }

            Text {
                text: "/"
                font.pixelSize: 24
                color: "#FFFFFF"
                Layout.alignment: Qt.AlignHCenter
            }

            TextField {
                id: denPolyField
                Layout.fillWidth: true
                placeholderText: "Q(x), например: x^2 - 1"
                placeholderTextColor: "#636366"
                text: denominatorPoly
                color: "#FFFFFF"
                font.pixelSize: 18
                font.family: "SF Pro Display"
                validator: RegularExpressionValidator { regularExpression: /^[-0-9xX^+*/()\s]*$/ }
                background: Rectangle {
                    color: surfaceColor
                    radius: 10
                }
                onTextChanged: denominatorPoly = text
            }
        }

        // Поля ввода для PCF -> P/Q и конвергентов
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12
            visible: currentMode === 1 || currentMode === 2 || currentMode === 3

            Text {
                text: currentMode === 1 ? "Цепная дробь [P₀; P₁, P₂, ...]" : 
                      currentMode === 2 ? "Цепная дробь для конвергентов" : "Цепная дробь для инвертирования"
                font.pixelSize: 14
                color: "#8E8E93"
                font.family: "SF Pro Display"
            }

            Text {
                text: "Введите многочлены через точку с запятой"
                font.pixelSize: 12
                color: "#636366"
                font.family: "SF Pro Display"
            }

            TextField {
                id: pcfField
                Layout.fillWidth: true
                placeholderText: "Например: x; 2x+1; x-1"
                placeholderTextColor: "#636366"
                text: pcfInput
                color: "#FFFFFF"
                font.pixelSize: 18
                font.family: "SF Pro Display"
                validator: RegularExpressionValidator { regularExpression: /^[-0-9xX^+*/();,\s]*$/ }
                background: Rectangle {
                    color: surfaceColor
                    radius: 10
                }
                onTextChanged: pcfInput = text
            }
        }

        Item { Layout.fillHeight: true }

        // Кнопки действий
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

        // Статус
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
            case 0: return "Преобразование P(x)/Q(x) в цепную дробь"
            case 1: return "Преобразование цепной дроби в P(x)/Q(x)"
            case 2: return "Вычисление конвергентов"
            case 3: return "Инвертирование цепной дроби (1/x)"
            default: return ""
        }
    }

    function clearInputs() {
        numeratorPoly = ""
        denominatorPoly = ""
        pcfInput = ""
        numPolyField.text = ""
        denPolyField.text = ""
        pcfField.text = ""
        
        // Сброс отображения
        resultLabel.text = "Введите данные"
        resultLabel.visible = true
        mathResultDisplay.expression = ""
        mathResultDisplay.visible = false
        multiLineResult.visible = false
        resultRepeater.model = []
        
        historyLabel.text = "Полиномиальные цепные дроби"
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
                case 0: // P/Q -> PCF
                    if (numeratorPoly === "" || denominatorPoly === "") {
                        showError("Введите P(x) и Q(x)")
                        return
                    }
                    result = calculatorBackend.polyToPCF(numeratorPoly, denominatorPoly)
                    historyLabel.text = "(" + numeratorPoly + ") / (" + denominatorPoly + ") ="
                    break

                case 1: // PCF -> P/Q
                    if (pcfInput === "") {
                        showError("Введите многочлены")
                        return
                    }
                    result = calculatorBackend.pcfToPoly(pcfInput)
                    historyLabel.text = "[" + pcfInput + "] ="
                    break

                case 2: // Конвергенты
                    if (pcfInput === "") {
                        showError("Введите многочлены")
                        return
                    }
                    result = calculatorBackend.pcfConvergents(pcfInput)
                    historyLabel.text = "Конвергенты:"
                    break

                case 3: // 1/x (инвертирование)
                    if (pcfInput === "") {
                        showError("Введите многочлены")
                        return
                    }
                    result = calculatorBackend.pcfInvert(pcfInput)
                    historyLabel.text = "1 / [" + pcfInput + "] ="
                    break
            }

            if (result.startsWith("Ошибка")) {
                showError("Ошибка")
            } else if (currentMode === 1 || currentMode === 2) {
                // PCF->P/Q и конвергенты - многострочный вывод
                var lines = result.split("\n")
                showMultiLineResult(lines)
            } else if (currentMode === 3) {
                // 1/x - однострочный вывод
                showSingleResult(result)
            } else {
                showSingleResult(result)
            }

        } catch (error) {
            showError("Ошибка")
        }
    }
}
