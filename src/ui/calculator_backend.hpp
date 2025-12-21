#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include "calculator.hpp"
#include "continued_fraction.hpp"

class CalculatorBackend : public QObject {
    Q_OBJECT

public:
    explicit CalculatorBackend(QObject *parent = nullptr);

    Q_INVOKABLE QString evaluate(const QString& expression);
    Q_INVOKABLE bool validate(const QString& expression);

    Q_INVOKABLE QString rationalToCF(const QString& numerator, const QString& denominator);
    Q_INVOKABLE QString cfToRational(const QString& coefficients);
    Q_INVOKABLE QString sqrtToCF(const QString& d);
    Q_INVOKABLE QString cfConvergents(const QString& coefficients);

    Q_INVOKABLE QString polyToPCF(const QString& numerator, const QString& denominator);
    Q_INVOKABLE QString pcfToPoly(const QString& polynomials);
    Q_INVOKABLE QString pcfConvergents(const QString& polynomials);

private:
    Calculator calculator_;
    
    QString convertFromQmlFormat(const QString& qmlExpression);
    QString formatResult(const QString& apiResult);
    QString formatPolynomialPretty(const QString& poly);
    std::vector<Integer> parseCoefficients(const QString& input);
    std::vector<Polynomial> parsePolynomials(const QString& input);
};

