#include "calculator_backend.hpp"
#include <QDebug>
#include <QRegularExpression>

CalculatorBackend::CalculatorBackend(QObject *parent)
    : QObject(parent), calculator_() {
}

QString CalculatorBackend::formatResult(const QString& apiResult) {
    QString result = apiResult;
    
    // Удаляем все пробелы
    result.replace(" ", "");
    
    // Заменяем "число*x" на "числоx" (убираем знак умножения перед переменной)
    QRegularExpression mulBeforeX("(\\d+)\\*x");
    result.replace(mulBeforeX, "\\1x");
    
    // Заменяем "*x" на "x" (для случаев без коэффициента)
    result.replace("*x", "x");
    
    // Заменяем "x*^" на "x^" (убираем лишний знак умножения перед степенью)
    result.replace("x*^", "x^");
    
    // Заменяем "x^1" на "x"
    QRegularExpression xPowerOne("x\\^1(?!\\d)");
    result.replace(xPowerOne, "x");
    
    // Убираем единичный коэффициент "1x" -> "x" (в начале или после операторов)
    result.replace(QRegularExpression("(^|[+\\-])1x"), "\\1x");
    
    // Заменяем "+x" в начале на "x" (убираем лишний плюс в начале)
    if (result.startsWith("+")) {
        result = result.mid(1);
    }
    
    qDebug() << "[Backend] Formatted result:" << result;
    
    return result;
}

QString CalculatorBackend::convertFromQmlFormat(const QString& qmlExpression) {
    QString result = qmlExpression;
    
    static const QMap<QChar, QString> superscriptMap = {
        {QChar(0x2070), "^0"}, // ⁰
        {QChar(0x00B9), "^1"}, // ¹
        {QChar(0x00B2), "^2"}, // ²
        {QChar(0x00B3), "^3"}, // ³
        {QChar(0x2074), "^4"}, // ⁴
        {QChar(0x2075), "^5"}, // ⁵
        {QChar(0x2076), "^6"}, // ⁶
        {QChar(0x2077), "^7"}, // ⁷
        {QChar(0x2078), "^8"}, // ⁸
        {QChar(0x2079), "^9"}  // ⁹
    };
    for (auto it = superscriptMap.constBegin(); it != superscriptMap.constEnd(); ++it) {
        result.replace(it.key(), it.value());
    }
    
    QRegularExpression numberBeforeBracket("(\\d)\\(");
    result.replace(numberBeforeBracket, "\\1*(");
    
    // проверяем, нет ли x в степени (x^x, x^2x и т.д.)
    QRegularExpression xInExponent("\\^[^0-9+\\-*/%()\\s]*x");
    if (xInExponent.match(result).hasMatch()) {
        throw std::invalid_argument("Недопустимо: x в показателе степени");
    }
    
    // Заменяем "xx" на "(x)*(x)" для умножения переменных
    QRegularExpression multipleXWithCoef("(\\d+(?:/\\d+)?)x{2,}");
    QRegularExpressionMatchIterator it1 = multipleXWithCoef.globalMatch(result);
    
    QList<QPair<int, QString>> replacements;
    while (it1.hasNext()) {
        QRegularExpressionMatch match = it1.next();
        QString coef = match.captured(1);
        int xCount = match.captured(0).count('x');
        
        // Создаём строку вида "k*(x)*(x)*(x)"
        QString replacement = coef;
        for (int i = 0; i < xCount; i++) {
            replacement += "*(x)";
        }
        
        replacements.prepend(qMakePair(match.capturedStart(), replacement));
    }
    
    for (const auto& repl : replacements) {
        int pos = result.indexOf(multipleXWithCoef, repl.first);
        if (pos != -1) {
            result.replace(pos, multipleXWithCoef.match(result, pos).capturedLength(), repl.second);
        }
    }
    
    QRegularExpression multipleX("(?<!\\d)x{2,}");
    QRegularExpressionMatchIterator it2 = multipleX.globalMatch(result);
    
    QList<QPair<int, int>> matches;
    while (it2.hasNext()) {
        QRegularExpressionMatch match = it2.next();
        matches.prepend(qMakePair(match.capturedStart(), match.capturedLength()));
    }
    
    for (const auto& match : matches) {
        int start = match.first;
        int length = match.second;
        
        QString replacement;
        for (int i = 0; i < length; i++) {
            if (i > 0) replacement += "*";
            replacement += "(x)";
        }
        
        result.replace(start, length, replacement);
    }
    
    qDebug() << "[Backend] Converted:" << qmlExpression << "->" << result;
    
    return result;
}

QString CalculatorBackend::evaluate(const QString& expression) {
    try {
        QString convertedExpr = convertFromQmlFormat(expression);
        
        // Удаляем все пробелы перед отправкой в API
        convertedExpr.replace(" ", "");

        // Для каждого числа без x добавляем *x^0
        if (!convertedExpr.contains('x') && !convertedExpr.contains('X')) {
            QRegularExpression numberPattern("(\\d+(?:/\\d+)?)");
            convertedExpr.replace(numberPattern, "(\\1)*x^0");
        }

        std::string stdExpression = convertedExpr.toUtf8().constData();
        
        qDebug() << "[Backend] Evaluating:" << QString::fromUtf8(stdExpression.c_str());

        Polynomial result = calculator_.simplify_expression(stdExpression);
        std::string resultStr = result.as_string();
        
        qDebug() << "[Backend] Raw result:" << QString::fromUtf8(resultStr.c_str());
        QString formattedResult = formatResult(QString::fromUtf8(resultStr.c_str()));

        return formattedResult;
        
    } catch (const std::exception& e) {
        QString errorMsg = QString::fromLocal8Bit(e.what());
        qDebug() << "[Backend] Error:" << errorMsg;
        return QString("Ошибка: %1").arg(errorMsg);
    } catch (...) {
        qDebug() << "[Backend] Unknown error";
        return QString("Ошибка: Неизвестная ошибка");
    }
}

bool CalculatorBackend::validate(const QString& expression) {
    try {
        QString convertedExpr = convertFromQmlFormat(expression);
        std::string stdExpression = convertedExpr.toUtf8().constData();

        calculator_.simplify_expression(stdExpression);
        return true;
        
    } catch (...) {
        return false;
    }
}

