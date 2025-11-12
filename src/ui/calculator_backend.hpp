#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include "calculator.hpp"

class CalculatorBackend : public QObject {
    Q_OBJECT

public:
    explicit CalculatorBackend(QObject *parent = nullptr);

    Q_INVOKABLE QString evaluate(const QString& expression);

    Q_INVOKABLE bool validate(const QString& expression);

private:
    Calculator calculator_;
    
    QString convertFromQmlFormat(const QString& qmlExpression);
    
    QString formatResult(const QString& apiResult);
};

