#include "calculator_backend.hpp"
#include <QDebug>
#include <QRegularExpression>
#include <QStringList>

CalculatorBackend::CalculatorBackend(QObject *parent)
    : QObject(parent), calculator_() {
}

std::vector<Integer> CalculatorBackend::parseCoefficients(const QString& input) {
    std::vector<Integer> result;
    QStringList parts = input.split(QRegularExpression("[,;\\s]+"), Qt::SkipEmptyParts);
    
    for (const QString& part : parts) {
        QString trimmed = part.trimmed();
        if (!trimmed.isEmpty()) {
            result.push_back(Integer(trimmed.toStdString()));
        }
    }
    
    return result;
}

QString CalculatorBackend::rationalToCF(const QString& numerator, const QString& denominator) {
    try {
        QString numStr = numerator.trimmed();
        QString denStr = denominator.trimmed();
        
        if (numStr.isEmpty() || denStr.isEmpty()) {
            return "Ошибка: введите числитель и знаменатель";
        }
        
        if (denStr == "0") {
            return "Ошибка: деление на ноль";
        }
        
        // Обработка знака знаменателя
        bool denNegative = denStr.startsWith("-");
        if (denNegative) {
            denStr = denStr.mid(1);
            // Меняем знак числителя
            if (numStr.startsWith("-")) {
                numStr = numStr.mid(1);
            } else {
                numStr = "-" + numStr;
            }
        }
        
        Integer num(numStr.toStdString());
        Natural den(denStr.toStdString());
        
        Rational r(num, den);
        
        ContinuedFraction cf = ContinuedFraction::FROM_Q_CF(r);
        return QString::fromStdString(cf.as_string());
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}

QString CalculatorBackend::cfToRational(const QString& coefficients) {
    try {
        std::vector<Integer> coefs = parseCoefficients(coefficients);
        
        if (coefs.empty()) {
            return "Ошибка: введите коэффициенты";
        }
        
        ContinuedFraction cf(coefs);
        Rational r = cf.TO_CF_Q();
        
        return QString::fromStdString(r.as_string());
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}

QString CalculatorBackend::sqrtToCF(const QString& d) {
    try {
        QString dStr = d.trimmed();
        
        if (dStr.isEmpty() || dStr == "0") {
            return "Ошибка: D должно быть положительным целым";
        }
        
        // Проверка на полный квадрат
        bool ok;
        unsigned long long dVal = dStr.toULongLong(&ok);
        if (ok) {
            unsigned long long sqrtD = static_cast<unsigned long long>(std::sqrt(static_cast<double>(dVal)));
            if (sqrtD * sqrtD == dVal) {
                return QString("[%1] (полный квадрат)").arg(sqrtD);
            }
        }
        
        ContinuedFraction cf = ContinuedFraction::FROM_SQRT_CF(Natural(dStr.toStdString()));
        return QString::fromStdString(cf.as_string());
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}

QString CalculatorBackend::cfConvergents(const QString& coefficients) {
    try {
        std::vector<Integer> coefs = parseCoefficients(coefficients);
        
        if (coefs.empty()) {
            return "Ошибка: введите коэффициенты";
        }
        
        ContinuedFraction cf(coefs);
        std::vector<Rational> convergents = cf.CONVERGENTS_CF();
        
        // Надстрочные цифры для индексов
        static const QString subscriptDigits[] = {
            QString::fromUtf8("₀"), QString::fromUtf8("₁"), QString::fromUtf8("₂"),
            QString::fromUtf8("₃"), QString::fromUtf8("₄"), QString::fromUtf8("₅"),
            QString::fromUtf8("₆"), QString::fromUtf8("₇"), QString::fromUtf8("₈"),
            QString::fromUtf8("₉")
        };
        
        auto toSubscript = [&](size_t n) -> QString {
            if (n == 0) return subscriptDigits[0];
            QString result;
            while (n > 0) {
                result.prepend(subscriptDigits[n % 10]);
                n /= 10;
            }
            return result;
        };
        
        QStringList result;
        for (size_t i = 0; i < convergents.size(); ++i) {
            result << QString("p%1/q%1 = %2")
                      .arg(toSubscript(i))
                      .arg(QString::fromStdString(convergents[i].as_string()));
        }
        
        return result.join("\n");
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}

QString CalculatorBackend::cfApprox(const QString& coefficients, const QString& maxDenominator) {
    try {
        std::vector<Integer> coefs = parseCoefficients(coefficients);
        
        if (coefs.empty()) {
            return "Ошибка: введите коэффициенты";
        }
        
        QString maxDenStr = maxDenominator.trimmed();
        if (maxDenStr.isEmpty() || maxDenStr == "0") {
            return "Ошибка: введите максимальный знаменатель";
        }
        
        ContinuedFraction cf(coefs);
        Natural maxDen(maxDenStr.toStdString());
        Rational approx = cf.APPROX_CF_Q(maxDen);
        
        return QString::fromStdString(approx.as_string());
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}

QString CalculatorBackend::cfInvert(const QString& coefficients) {
    try {
        std::vector<Integer> coefs = parseCoefficients(coefficients);
        
        if (coefs.empty()) {
            return "Ошибка: введите коэффициенты";
        }
        
        ContinuedFraction cf(coefs);
        ContinuedFraction inverted = cf.INV_CF_CF();
        
        return QString::fromStdString(inverted.as_string());
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}

QString CalculatorBackend::cfToQuad(const QString& coefficients, int periodStart) {
    try {
        std::vector<Integer> coefs = parseCoefficients(coefficients);
        
        if (coefs.empty()) {
            return "Ошибка: введите коэффициенты";
        }
        
        ContinuedFraction cf(coefs, periodStart);
        auto [a, D, c] = cf.TO_CF_QUAD();
        
        QString result;
        if (D.COM_NN_D(Natural("0")) == 0) {
            if (c.COM_ZZ_D(Integer("1")) == 0) {
                result = QString::fromStdString(a.as_string());
            } else {
                result = QString("%1/%2").arg(QString::fromStdString(a.as_string()))
                                         .arg(QString::fromStdString(c.as_string()));
            }
        } else {
            // Квадратичная иррациональность: (a + √D) / c
            QString aStr = QString::fromStdString(a.as_string());
            QString dStr = QString::fromStdString(D.as_string());
            QString cStr = QString::fromStdString(c.as_string());
            
            if (c.COM_ZZ_D(Integer("1")) == 0) {
                if (a.SGN_Z_D() == 0) {
                    result = QString("√%1").arg(dStr);
                } else {
                    result = QString("%1 + √%2").arg(aStr).arg(dStr);
                }
            } else {
                if (a.SGN_Z_D() == 0) {
                    result = QString("√%1 / %2").arg(dStr).arg(cStr);
                } else {
                    result = QString("(%1 + √%2) / %3").arg(aStr).arg(dStr).arg(cStr);
                }
            }
        }
        
        return result;
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
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
    result.replace(QRegularExpression("(^|[+\\-\\(])1x"), "\\1x");
    result.replace(QRegularExpression("([,;])1x"), "\\1x");
    
    // Заменяем "+x" в начале на "x" (убираем лишний плюс в начале)
    if (result.startsWith("+")) {
        result = result.mid(1);
    }

    qDebug() << "[Backend] Formatted result:" << result;
    
    return result;
}

QString CalculatorBackend::formatPolynomialPretty(const QString& poly) {
    QString result = formatResult(poly);
    
    // Заменяем степени на надстрочные символы
    static const QMap<QChar, QChar> superscriptDigits = {
        {'0', QChar(0x2070)}, // ⁰
        {'1', QChar(0x00B9)}, // ¹
        {'2', QChar(0x00B2)}, // ²
        {'3', QChar(0x00B3)}, // ³
        {'4', QChar(0x2074)}, // ⁴
        {'5', QChar(0x2075)}, // ⁵
        {'6', QChar(0x2076)}, // ⁶
        {'7', QChar(0x2077)}, // ⁷
        {'8', QChar(0x2078)}, // ⁸
        {'9', QChar(0x2079)}  // ⁹
    };
    
    // Находим все ^число и заменяем на надстрочные
    QRegularExpression powerRegex("\\^(\\d+)");
    QRegularExpressionMatchIterator it = powerRegex.globalMatch(result);
    
    // Собираем замены в обратном порядке
    QList<QPair<int, QPair<int, QString>>> replacements;
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString digits = match.captured(1);
        QString superscript;
        for (const QChar& c : digits) {
            if (superscriptDigits.contains(c)) {
                superscript += superscriptDigits[c];
            } else {
                superscript += c;
            }
        }
        replacements.prepend(qMakePair(match.capturedStart(), qMakePair(match.capturedLength(), superscript)));
    }
    
    // Применяем замены
    for (const auto& repl : replacements) {
        result.replace(repl.first, repl.second.first, repl.second.second);
    }
    
    return result;
}

QString CalculatorBackend::convertFromQmlFormat(const QString& qmlExpression) {
    QString result = qmlExpression;
    
    result = result.simplified();
    result.replace(" ", "");
    
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
    
    // TODO: калькулятор не должен этим заниматься

    QRegularExpression numberBeforeBracket("(\\d)\\(");
    result.replace(numberBeforeBracket, "\\1*(");
    
    // проверяем, нет ли x в степени (x^x, x^2x и т.д.)
    QRegularExpression xInExponent("\\^[^0-9+\\-*/%()\\s]*x");
    if (xInExponent.match(result).hasMatch()) {
        throw std::invalid_argument("Недопустимо: x в показателе степени");
    }
    
    // Заменяем "xx" на "x*x" для умножения переменных
    QRegularExpression multipleXWithCoef("(\\d+(?:/\\d+)?)x{2,}");
    QRegularExpressionMatchIterator it1 = multipleXWithCoef.globalMatch(result);
    
    QList<QPair<int, QString>> replacements;
    while (it1.hasNext()) {
        QRegularExpressionMatch match = it1.next();
        QString coef = match.captured(1);
        int xCount = match.captured(0).count('x');
        
        // Создаём строку вида "k*x*x*x"
        QString replacement = coef;
        for (int i = 0; i < xCount; i++) {
            replacement += "*x";
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
            replacement += "x";
        }
        
        result.replace(start, length, replacement);
    }
    
    qDebug() << "[Backend] Converted:" << qmlExpression << "->" << result;
    
    return result;
}

QString CalculatorBackend::evaluate(const QString& expression) {
    try {
        QString convertedExpr = convertFromQmlFormat(expression);

        std::string stdExpression = convertedExpr.toUtf8().constData();
        
        qDebug() << "[Backend] Evaluating:" << QString::fromUtf8(stdExpression.c_str());

        std::string result = calculator_.simplify_expression(stdExpression);
        
        qDebug() << "[Backend] Raw result:" << QString::fromUtf8(result.c_str());
        
        if (result.find("Error:") == 0) {
            return QString("Ошибка");
        }
        
        QString formattedResult = formatPolynomialPretty(QString::fromUtf8(result.c_str()));

        return formattedResult;
        
    } catch (const std::exception& e) {
        QString errorMsg = QString::fromLocal8Bit(e.what());
        qDebug() << "[Backend] Error:" << errorMsg;
        return QString("Ошибка");
    } catch (...) {
        qDebug() << "[Backend] Unknown error";
        return QString("Ошибка");
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

std::vector<Polynomial> CalculatorBackend::parsePolynomials(const QString& input) {
    std::vector<Polynomial> result;
    QStringList parts = input.split(";", Qt::SkipEmptyParts);
    
    for (const QString& part : parts) {
        QString trimmed = part.trimmed();
        if (!trimmed.isEmpty()) {
            QString converted = convertFromQmlFormat(trimmed);
            result.push_back(calculator_.parse_expression(converted.toStdString()));
        }
    }
    
    return result;
}

QString CalculatorBackend::polyToPCF(const QString& numerator, const QString& denominator) {
    try {
        QString numStr = numerator.trimmed();
        QString denStr = denominator.trimmed();
        
        if (numStr.isEmpty() || denStr.isEmpty()) {
            return "Ошибка: введите P(x) и Q(x)";
        }
        
        QString numConverted = convertFromQmlFormat(numStr);
        QString denConverted = convertFromQmlFormat(denStr);
        
        Polynomial P = calculator_.parse_expression(numConverted.toStdString());
        Polynomial Q = calculator_.parse_expression(denConverted.toStdString());
        
        PolynomialContinuedFraction pcf = PolynomialContinuedFraction::FROM_PQ_PCF(P, Q);
        QString raw = QString::fromStdString(pcf.as_string());
        
        // Форматируем каждый полином в цепной дроби
        // Формат: [P0; P1, P2, ...]
        // Разбиваем и форматируем
        raw.replace("[", "");
        raw.replace("]", "");
        QStringList parts = raw.split(QRegularExpression("[;,]"));
        QStringList formatted;
        for (int i = 0; i < parts.size(); ++i) {
            formatted << formatPolynomialPretty(parts[i].trimmed());
        }
        
        if (formatted.isEmpty()) {
            return "[]";
        }
        
        QString result = "[" + formatted[0];
        for (int i = 1; i < formatted.size(); ++i) {
            result += (i == 1 ? "; " : ", ") + formatted[i];
        }
        result += "]";
        
        return result;
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}

QString CalculatorBackend::pcfToPoly(const QString& polynomials) {
    try {
        std::vector<Polynomial> polys = parsePolynomials(polynomials);
        
        if (polys.empty()) {
            return "Ошибка: введите многочлены";
        }
        
        PolynomialContinuedFraction pcf(polys);
        auto [P, Q] = pcf.TO_PCF_PQ();
        
        QString pFormatted = formatPolynomialPretty(QString::fromStdString(P.as_string()));
        QString qFormatted = formatPolynomialPretty(QString::fromStdString(Q.as_string()));
        
        QString result = QString("P(x) = %1\nQ(x) = %2")
            .arg(pFormatted)
            .arg(qFormatted);
        
        return result;
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}

QString CalculatorBackend::pcfConvergents(const QString& polynomials) {
    try {
        std::vector<Polynomial> polys = parsePolynomials(polynomials);
        
        if (polys.empty()) {
            return "Ошибка: введите многочлены";
        }
        
        PolynomialContinuedFraction pcf(polys);
        auto convergents = pcf.CONVERGENTS_PCF();
        
        // Подстрочные цифры для индексов
        static const QString subscriptDigits[] = {
            QString::fromUtf8("₀"), QString::fromUtf8("₁"), QString::fromUtf8("₂"),
            QString::fromUtf8("₃"), QString::fromUtf8("₄"), QString::fromUtf8("₅"),
            QString::fromUtf8("₆"), QString::fromUtf8("₇"), QString::fromUtf8("₈"),
            QString::fromUtf8("₉")
        };
        
        auto toSubscript = [&](size_t n) -> QString {
            if (n == 0) return subscriptDigits[0];
            QString result;
            while (n > 0) {
                result.prepend(subscriptDigits[n % 10]);
                n /= 10;
            }
            return result;
        };
        
        QStringList result;
        for (size_t i = 0; i < convergents.size(); ++i) {
            QString pStr = formatPolynomialPretty(QString::fromStdString(convergents[i].first.as_string()));
            QString qStr = formatPolynomialPretty(QString::fromStdString(convergents[i].second.as_string()));
            result << QString("C%1: %2 / %3")
                      .arg(toSubscript(i))
                      .arg(pStr)
                      .arg(qStr);
        }
        
        return result.join("\n");
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}


QString CalculatorBackend::pcfInvert(const QString& polynomials) {
    try {
        std::vector<Polynomial> polys = parsePolynomials(polynomials);
        
        if (polys.empty()) {
            return "Ошибка: введите многочлены";
        }
        
        PolynomialContinuedFraction pcf(polys);
        PolynomialContinuedFraction inverted = pcf.INV_PCF_PCF();
        
        // Форматируем результат
        QString raw = QString::fromStdString(inverted.as_string());
        raw.replace("[", "");
        raw.replace("]", "");
        raw.replace("(", "");
        raw.replace(")", "");
        QStringList parts = raw.split(QRegularExpression("[;,]"));
        QStringList formatted;
        for (const QString& part : parts) {
            formatted << formatPolynomialPretty(part.trimmed());
        }
        
        if (formatted.isEmpty()) {
            return "[]";
        }
        
        QString result = "[" + formatted[0];
        for (int i = 1; i < formatted.size(); ++i) {
            result += (i == 1 ? "; " : ", ") + formatted[i];
        }
        result += "]";
        
        return result;
        
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(QString::fromLocal8Bit(e.what()));
    }
}
