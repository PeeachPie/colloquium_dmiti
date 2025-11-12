pragma Singleton
import QtQuick

/*
 * Singleton для управления темой приложения
 */
QtObject {
    id: theme
    
    // Цвета фона
    readonly property color backgroundColor: "#000000"
    readonly property color surfaceColor: "#1C1C1E"
    readonly property color surfaceColorLight: "#2C2C2E"
    
    // Цвета кнопок
    readonly property color buttonDefault: "#333333"
    readonly property color buttonOperator: "#FF9500"
    readonly property color buttonSpecial: "#505050"
    readonly property color buttonEquals: "#34C759"
    readonly property color buttonClear: "#FF3B30"
    readonly property color buttonFunction: "#007AFF"
    
    // Цвета текста
    readonly property color textPrimary: "#FFFFFF"
    readonly property color textSecondary: "#8E8E93"
    readonly property color textTertiary: "#636366"
    
    // Акцентные цвета
    readonly property color accentPrimary: "#007AFF"
    readonly property color accentSuccess: "#34C759"
    readonly property color accentWarning: "#FF9500"
    readonly property color accentError: "#FF3B30"
    
    // Размеры
    readonly property real buttonRadius: 12
    readonly property real surfaceRadius: 16
    readonly property real spacing: 12
    readonly property real margin: 20
    
    // Шрифты
    readonly property string fontFamily: "SF Pro Display"
    readonly property string fontFamilyMono: "SF Mono"
    
    readonly property real fontSizeExtraLarge: 48
    readonly property real fontSizeLarge: 26
    readonly property real fontSizeMedium: 20
    readonly property real fontSizeSmall: 16
    readonly property real fontSizeExtraSmall: 12
    
    // Анимации
    readonly property int animationDurationFast: 100
    readonly property int animationDurationMedium: 200
    readonly property int animationDurationSlow: 300
    
    // Тени
    readonly property color shadowColor: "#40000000"
    readonly property real shadowBlur: 0.4
    readonly property real shadowOffset: 3
    
    function getButtonColor(type) {
        switch(type) {
            case "operator": return buttonOperator
            case "special": return buttonSpecial
            case "equals": return buttonEquals
            case "clear": return buttonClear
            case "function": return buttonFunction
            default: return buttonDefault
        }
    }
    
    function withOpacity(color, opacity) {
        return Qt.rgba(color.r, color.g, color.b, opacity)
    }
    
    function lighten(color, factor) {
        return Qt.lighter(color, factor)
    }
    
    function darken(color, factor) {
        return Qt.darker(color, factor)
    }
    
    // TODO: light theme
    property bool isDarkTheme: true

    /*
    readonly property color backgroundColorLight: "#F2F2F7"
    readonly property color surfaceColorLight: "#FFFFFF"
    readonly property color textPrimaryLight: "#000000"
    */
}


