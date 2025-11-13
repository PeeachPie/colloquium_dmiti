#include <iostream>

#include "natural.hpp"
#include "integer.hpp"
#include "rational.hpp"
#include "polynomial.hpp"

// Qt для GUI (если доступен)
#ifdef USE_QT_GUI
    #include <QGuiApplication>
    #include <QQmlApplicationEngine>
    #include <QQmlContext>
    #include <QIcon>
    #include "calculator_backend.hpp"
#endif

int main(int argc, char *argv[]) {
#ifdef USE_QT_GUI
    // Если Qt доступен, запускаем GUI версию
    QGuiApplication app(argc, argv);
    
    app.setOrganizationName("4383");
    app.setOrganizationDomain("cas.dmiti");
    app.setApplicationName("CAS Calculator");

    CalculatorBackend calculatorBackend;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("calculatorBackend", &calculatorBackend);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    
    QObject::connect(
        &engine, 
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection
    );
    
    engine.load(url);
    
    if (engine.rootObjects().isEmpty()) {
        std::cerr << "Ошибка: не удалось загрузить QML интерфейс" << std::endl;
        return -1;
    }
    
    std::cout << "=== CAS Calculator GUI запущен ===" << std::endl;
    std::cout << "Используйте графический интерфейс для работы с калькулятором" << std::endl;
    
    return app.exec();
    
#else
    // Консольная версия (без Qt)
    std::cout << "=== CAS Calculator - Консольная версия ===" << std::endl;
    std::cout << "Qt не найден, запускаем демо-режим\n" << std::endl;
    
    std::cout << "=== Тестирование Natural ===" << std::endl;
    Natural a = Natural("893");
    Natural b = Natural("10");
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "a + b = " << a.ADD_NN_N(b) << std::endl;
    
    std::cout << "\n=== Тестирование Integer ===" << std::endl;
    Integer x("-5");
    Integer y("3");
    std::cout << "x = " << x << ", y = " << y << std::endl;
    std::cout << "x + y = " << x.ADD_ZZ_Z(y) << std::endl;
    
    std::cout << "\n=== Тестирование Rational ===" << std::endl;
    Rational r1("1/2");
    Rational r2("1/3");
    std::cout << "r1 = " << r1 << ", r2 = " << r2 << std::endl;
    std::cout << "r1 + r2 = " << r1.ADD_QQ_Q(r2) << std::endl;
    
    Rational r3("6/8");
    std::cout << "r3 = " << r3 << " -> ";
    r3.RED_Q_Q();
    std::cout << "после сокращения: " << r3 << std::endl;
    
    std::cout << "\n=== Для использования GUI установите Qt6 ===" << std::endl;
    
    return 0;
#endif
}
