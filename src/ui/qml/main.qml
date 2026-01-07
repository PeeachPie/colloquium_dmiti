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
    
    property int currentTab: 0  // 0: Полиномы, 1: Цепные дроби, 2: Полиномиальные ЦД
    
    color: backgroundColor
    
    Drawer {
        id: drawer
        width: 280
        height: mainWindow.height
        
        background: Rectangle {
            color: surfaceColor
        }
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 0
            spacing: 0
            
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                color: backgroundColor
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 4
                    
                    Item { Layout.fillHeight: true }
                    
                    Text {
                        text: "4383"
                        font.pixelSize: 28
                        font.family: "SF Pro Display"
                        font.weight: Font.Bold
                        color: "#FFFFFF"
                        renderType: Text.NativeRendering
                    }
                    
                    Text {
                        text: "Calculator"
                        font.pixelSize: 16
                        font.family: "SF Pro Display"
                        color: "#8E8E93"
                        renderType: Text.NativeRendering
                    }
                }
            }
            
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: "#3A3A3C"
            }
            
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: 12
                
                model: ListModel {
                    ListElement { name: "Полиномы"; icon: "𝑥"; tabIndex: 0 }
                    ListElement { name: "Цепные дроби"; icon: "∞"; tabIndex: 1 }
                    ListElement { name: "Полином. ЦД"; icon: "𝑓"; tabIndex: 2 }
                }
                
                delegate: Rectangle {
                    width: ListView.view.width
                    height: 56
                    color: currentTab === tabIndex ? Qt.rgba(0, 122/255, 255/255, 0.2) : "transparent"
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 20
                        anchors.rightMargin: 20
                        spacing: 16
                        
                        // Индикатор выбора
                        Rectangle {
                            width: 4
                            height: 32
                            radius: 2
                            color: currentTab === tabIndex ? primaryColor : "transparent"
                        }
                        
                        // Иконка
                        Text {
                            text: icon
                            font.pixelSize: 24
                            font.family: "SF Pro Display"
                            color: currentTab === tabIndex ? primaryColor : "#8E8E93"
                            renderType: Text.NativeRendering
                        }
                        
                        // Название
                        Text {
                            Layout.fillWidth: true
                            text: name
                            font.pixelSize: 17
                            font.family: "SF Pro Display"
                            font.weight: currentTab === tabIndex ? Font.Medium : Font.Normal
                            color: currentTab === tabIndex ? "#FFFFFF" : "#EBEBF5"
                            renderType: Text.NativeRendering
                        }
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            currentTab = tabIndex
                            drawer.close()
                        }
                    }
                }
            }
            
            // Нижняя часть меню
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                color: "transparent"
                
                Text {
                    anchors.centerIn: parent
                    text: "Версия 1.2"
                    font.pixelSize: 12
                    font.family: "SF Pro Display"
                    color: "#636366"
                    renderType: Text.NativeRendering
                }
            }
        }
    }
    
    // Основной контент
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        // Верхняя панель с кнопкой меню
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            color: backgroundColor
            
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                
                // Кнопка меню (гамбургер)
                Rectangle {
                    width: 40
                    height: 40
                    radius: 20
                    color: menuButton.pressed ? surfaceColor : "transparent"
                    
                    MouseArea {
                        id: menuButton
                        anchors.fill: parent
                        onClicked: drawer.open()
                    }
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 5
                        
                        Repeater {
                            model: 3
                            Rectangle {
                                width: 20
                                height: 2
                                radius: 1
                                color: "#FFFFFF"
                            }
                        }
                    }
                }
                
                // Заголовок текущей вкладки
                Text {
                    Layout.fillWidth: true
                    text: {
                        switch(currentTab) {
                            case 0: return "Полиномы"
                            case 1: return "Цепные дроби"
                            case 2: return "Полином. ЦД"
                            default: return ""
                        }
                    }
                    font.pixelSize: 20
                    font.family: "SF Pro Display"
                    font.weight: Font.Medium
                    color: "#FFFFFF"
                    horizontalAlignment: Text.AlignHCenter
                    renderType: Text.NativeRendering
                }
                
                // Пустой элемент для симметрии
                Item {
                    width: 40
                    height: 40
                }
            }
        }
        
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: currentTab
            
            PolynomialTab {
                id: polynomialTab
            }
            
            ContinuedFractionTab {
                id: cfTab
            }
            
            PolynomialCFTab {
                id: pcfTab
            }
        }
    }
    
    Shortcut {
        sequence: "Escape"
        onActivated: {
            if (drawer.opened) {
                drawer.close()
            } else if (currentTab === 0) {
                polynomialTab.clearAll()
            } else if (currentTab === 1) {
                cfTab.clearInputs()
            } else {
                pcfTab.clearInputs()
            }
        }
    }
    
    Shortcut {
        sequence: "Backspace"
        onActivated: {
            if (currentTab === 0) {
                polynomialTab.backspace()
            }
        }
    }
    
    Shortcut {
        sequence: "Return"
        onActivated: {
            if (currentTab === 0) {
                polynomialTab.calculate()
            } else if (currentTab === 1) {
                cfTab.calculate()
            } else {
                pcfTab.calculate()
            }
        }
    }
    
    Shortcut {
        sequence: "Enter"
        onActivated: {
            if (currentTab === 0) {
                polynomialTab.calculate()
            } else if (currentTab === 1) {
                cfTab.calculate()
            } else {
                pcfTab.calculate()
            }
        }
    }

    Item {
        focus: currentTab === 0
        anchors.fill: parent
        
        Keys.onPressed: function(event) {
            if (currentTab !== 0) return
            
            var key = event.text
            
            if (key >= '0' && key <= '9') {
                polynomialTab.handleInput(key)
                event.accepted = true
            }
            else if (key === '+') {
                polynomialTab.handleOperation("+")
                event.accepted = true
            }
            else if (key === '-') {
                polynomialTab.handleOperation("-")
                event.accepted = true
            }
            else if (key === '*') {
                polynomialTab.handleOperation("*")
                event.accepted = true
            }
            else if (key === '%') {
                polynomialTab.handleOperation("%")
                event.accepted = true
            }
            else if (key === '/') {
                polynomialTab.handleOperation("/")
                event.accepted = true
            }
            else if (key === 'x' || key === 'X') {
                polynomialTab.handleInput("x")
                event.accepted = true
            }
            else if (key === '(' || event.key === Qt.Key_ParenLeft) {
                polynomialTab.handleInput("(")
                event.accepted = true
            }
            else if (key === ')' || event.key === Qt.Key_ParenRight) {
                polynomialTab.handleInput(")")
                event.accepted = true
            }
            else if (key === '^') {
                polynomialTab.handleInput("^")
                event.accepted = true
            }
            else if (key === ',' || event.key === Qt.Key_Comma) {
                polynomialTab.handleInput(",")
                event.accepted = true
            }
        }
    }
}
