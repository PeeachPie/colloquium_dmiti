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
    property int currentMode: 0  // 0: P/Q->PCF, 1: PCF->P/Q, 2: конвергенты

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
                }
            }
        }

        // Выбор режима
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            spacing: 8

            Repeater {
                model: ["P/Q→PCF", "PCF→P/Q", "Конвергенты"]

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
            visible: currentMode === 1 || currentMode === 2

            Text {
                text: currentMode === 1 ? "Цепная дробь [P₀; P₁, P₂, ...]" : "Цепная дробь для конвергентов"
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
        resultLabel.text = "Введите данные"
        historyLabel.text = "Полиномиальные цепные дроби"
    }

    function calculate() {
        var result = ""

        try {
            switch(currentMode) {
                case 0: // P/Q -> PCF
                    if (numeratorPoly === "" || denominatorPoly === "") {
                        resultLabel.text = "Введите P(x) и Q(x)"
                        return
                    }
                    result = calculatorBackend.polyToPCF(numeratorPoly, denominatorPoly)
                    historyLabel.text = "(" + numeratorPoly + ") / (" + denominatorPoly + ") ="
                    break

                case 1: // PCF -> P/Q
                    if (pcfInput === "") {
                        resultLabel.text = "Введите многочлены"
                        return
                    }
                    result = calculatorBackend.pcfToPoly(pcfInput)
                    historyLabel.text = "[" + pcfInput + "] ="
                    break

                case 2: // Конвергенты
                    if (pcfInput === "") {
                        resultLabel.text = "Введите многочлены"
                        return
                    }
                    result = calculatorBackend.pcfConvergents(pcfInput)
                    historyLabel.text = "Конвергенты:"
                    break
            }

            if (result.startsWith("Ошибка")) {
                resultLabel.text = "Ошибка"
            } else {
                resultLabel.text = result
            }

        } catch (error) {
            resultLabel.text = "Ошибка"
        }
    }
}
