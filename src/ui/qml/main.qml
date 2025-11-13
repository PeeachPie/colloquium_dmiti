import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import "components"

/*
 * Главное окно калькулятора
 */
ApplicationWindow {
    id: mainWindow
    
    width: 420
    height: 720
    minimumWidth: 380
    minimumHeight: 650
    
    visible: true
    title: qsTr("4383 Calculator")
    
    readonly property color backgroundColor: "#000000"
    readonly property color surfaceColor: "#1C1C1E"
    readonly property color primaryColor: "#007AFF"
    
    property string currentInput: "0"
    property string currentOperation: ""
    property bool newInput: true
    
    // токены для группового удаления
    property var groupTokens: ["GCD(", "DER(", "FAC(", "NMR(", "^2", "^3", "^4", "^5", "^6", "^7", "^8", "^9", ",", "Ошибка"]
    
    color: backgroundColor
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20
        
        Display {
            id: display
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            
            mainText: currentInput
            historyText: "Добро пожаловать в калькулятор!\nВведите полиномиальное выражение"
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: surfaceColor
            radius: 12
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 8
                
                CalculatorButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    buttonText: "x"
                    buttonType: "function"
                    fontSize: 24
                    onClicked: handleInput("x")
                }
                
                CalculatorButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    buttonText: "xⁿ"
                    buttonType: "function"
                    fontSize: 20
                    onClicked: handleInput("x^")
                }

                CalculatorButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    buttonText: "("
                    buttonType: "function"
                    fontSize: 22
                    onClicked: handleInput("(")
                }

                CalculatorButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    buttonText: ")"
                    buttonType: "function"
                    fontSize: 22
                    onClicked: handleInput(")")
                }
            }
        }
        
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 4
            rowSpacing: 12
            columnSpacing: 12
            
            // Ряд 1: Очистка и операции
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "AC"
                buttonType: "clear"
                fontSize: 22
                onClicked: clearAll()
            }

            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "%"
                buttonType: "operator"
                fontSize: 24
                onClicked: handleOperation("%")
            }

            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "÷"
                buttonType: "operator"
                fontSize: 28
                onClicked: handleOperation("/")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "⌫"
                buttonType: "special"
                fontSize: 24
                onClicked: backspace()
            }
            
            // Ряд 2: 7, 8, 9, ×
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "7"
                fontSize: 26
                onClicked: handleInput("7")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "8"
                fontSize: 26
                onClicked: handleInput("8")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "9"
                fontSize: 26
                onClicked: handleInput("9")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "×"
                buttonType: "operator"
                fontSize: 26
                onClicked: handleOperation("*")
            }
            
            // Ряд 3: 4, 5, 6, −
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "4"
                fontSize: 26
                onClicked: handleInput("4")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "5"
                fontSize: 26
                onClicked: handleInput("5")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "6"
                fontSize: 26
                onClicked: handleInput("6")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "−"
                buttonType: "operator"
                fontSize: 28
                onClicked: handleOperation("-")
            }
            
            // Ряд 4: 1, 2, 3, +
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "1"
                fontSize: 26
                onClicked: handleInput("1")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "2"
                fontSize: 26
                onClicked: handleInput("2")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "3"
                fontSize: 26
                onClicked: handleInput("3")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "+"
                buttonType: "operator"
                fontSize: 28
                onClicked: handleOperation("+")
            }
            
            // Ряд 5: 0, запятая, =
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.columnSpan: 2
                buttonText: "0"
                fontSize: 26
                onClicked: handleInput("0")
            }

            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: ","
                fontSize: 24
                textVerticalOffset: -8
                onClicked: handleInput(",")
            }
            
            CalculatorButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                buttonText: "="
                buttonType: "equals"
                fontSize: 28
                onClicked: calculate()
            }
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            color: surfaceColor
            radius: 12
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 6
                spacing: 6
                
                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 6
                    
                    CalculatorButton {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        buttonText: "GCD"
                        buttonType: "function"
                        fontSize: 16
                        onClicked: handleFunction("gcd")
                    }
                    
                    CalculatorButton {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        buttonText: "DER"
                        buttonType: "function"
                        fontSize: 16
                        onClicked: handleFunction("derivative")
                    }
                }
                
                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 6
                    
                    CalculatorButton {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        buttonText: "FAC"
                        buttonType: "function"
                        fontSize: 16
                        onClicked: handleFunction("factorize")
                    }
                    
                    CalculatorButton {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        buttonText: "NMR"
                        buttonType: "function"
                        fontSize: 16
                        onClicked: handleFunction("nmr")
                    }
                }
            }
        }
        
        // Статус-бар
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "transparent"
            
            Text {
                id: statusText
                anchors.centerIn: parent
                text: "Работает с рациональными полиномами"
                font.pixelSize: 12
                font.family: "SF Pro Display"
                color: "#8E8E93"
                renderType: Text.NativeRendering
            }
        }
    }
    
    function handleInput(input) {
        if (newInput) {
            currentInput = input
            newInput = false
        } else {
            if (currentInput === "0" && input !== ",") {
                currentInput = input
            } else {
                currentInput += input
            }
        }
        display.mainText = currentInput
    }
    
    function handleOperation(operation) {
        currentOperation = operation
        if (operation === "/") {
            currentInput += operation
        } else {
            currentInput += " " + operation + " "
        }
        display.mainText = currentInput
        newInput = false
    }
    
    function handleFunction(funcName) {
        var funcDisplay = {
            "gcd": "GCD(",
            "derivative": "DER(",
            "factorize": "FAC(",
            "nmr": "NMR("
        }
        
        if (currentInput === "0" || newInput) {
            currentInput = funcDisplay[funcName] || funcName + "("
        } else {
            currentInput += funcDisplay[funcName] || funcName + "("
        }
        
        display.mainText = currentInput
        newInput = false
        
        display.historyText += "\nФункция " + funcDisplay[funcName] + " выбрана"
    }
    
    function calculate() {
        var expression = currentInput
        
        try {
            var result = calculatorBackend.evaluate(expression)
            
            if (result.startsWith("Ошибка:")) {
                display.historyText = expression
                display.mainText = "Ошибка"
            } else {
                display.historyText = expression + " ="
                display.mainText = result
                currentInput = result
            }
            
            newInput = true
            
            display.triggerUpdate()
            
        } catch (error) {
            display.historyText = expression
            display.mainText = "Ошибка"
            newInput = true
        }
    }
    
    function clearAll() {
        currentInput = "0"
        currentOperation = ""
        display.clear()
        newInput = true
    }
    
    function backspace() {
        if (currentInput.length <= 1) {
            currentInput = "0"
            display.mainText = currentInput
            return
        }
        
        var deletedToken = false
        for (var i = 0; i < groupTokens.length; i++) {
            var token = groupTokens[i]
            if (currentInput.endsWith(token)) {
                currentInput = currentInput.slice(0, -token.length)
                deletedToken = true
                break
            }
        }
        
        if (!deletedToken) {
            currentInput = currentInput.slice(0, -1)
        }
        
        if (currentInput.length === 0) {
            currentInput = "0"
        }
        
        display.mainText = currentInput
    }
    
    Shortcut {
        sequence: "Escape"
        onActivated: clearAll()
    }
    
    Shortcut {
        sequence: "Backspace"
        onActivated: backspace()
    }
    
    Shortcut {
        sequence: "Return"
        onActivated: calculate()
    }
    
    Shortcut {
        sequence: "Enter"
        onActivated: calculate()
    }
    
    Item {
        focus: true
        anchors.fill: parent
        
        Keys.onPressed: function(event) {
            var key = event.text
            
            if (key >= '0' && key <= '9') {
                handleInput(key)
                event.accepted = true
            }
            else if (key === '+') {
                handleOperation("+")
                event.accepted = true
            }
            else if (key === '-') {
                handleOperation("-")
                event.accepted = true
            }
            else if (key === '*') {
                handleOperation("*")
                event.accepted = true
            }
            else if (key === '%') {
                handleOperation("%")
                event.accepted = true
            }
            else if (key === '/') {
                handleOperation("/")
                event.accepted = true
            }
            else if (key === 'x' || key === 'X') {
                handleInput("x")
                event.accepted = true
            }
            else if (key === '(' || event.key === Qt.Key_ParenLeft) {
                handleInput("(")
                event.accepted = true
            }
            else if (key === ')' || event.key === Qt.Key_ParenRight) {
                handleInput(")")
                event.accepted = true
            }
            else if (key === '^') {
                handleInput("^")
                event.accepted = true
            }
            else if (key === ',' || event.key === Qt.Key_Comma) {
                handleInput(",")
                event.accepted = true
            }
        }
    }
}

