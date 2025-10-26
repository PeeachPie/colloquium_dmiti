# colloquium_dmiti

Цель [коллоквиума](https://docs.google.com/document/d/1Dv_6AIhxg_3ezu6VMcEnMpyfRzgym9l8PmE4ULGfjgM/edit?tab=t.0) - создать систему компьютерной алгебры

### Сборка и запуск проекта

Перейти в директорию build (при необходимости создать) и выполнить следующие команды:

1. Создать Makefile
```bash
cmake ..
```

2. Собрать исполняемый файл
```bash
make
```

3. Запустить программу
```bash
./app
```

Или же просто
```bash
cmake .. && make && ./app
```

### Запуск тестов

Чтобы запустить тесты необходимо собрать исполняемый файл и выполнить команду
```bash
ctest
```

При разработке вас будут интересовать не все тесты, их можно отфильтровать регулярным выражением
```bash
ctest -R "NaturalTests.CreateDefault"
```

Или вместе со сборкой
```bash
cmake .. && make && ctest -R "NaturalTests.CreateDefault"
```

Возможный вывод:
```bash
Test project /colloquium_dmiti/build
    Start 1: NaturalTests.CreateDefault
1/2 Test #1: NaturalTests.CreateDefault .......   Passed    0.00 sec
    Start 2: NaturalTests.InvalidString
2/2 Test #2: NaturalTests.InvalidString .......   Passed    0.00 sec

100% tests passed, 0 tests failed out of 2

Total Test time (real) =   0.01 sec
```