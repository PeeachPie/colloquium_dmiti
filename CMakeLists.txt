cmake_minimum_required(VERSION 3.12)
project(Algebra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /Zc:__cplusplus")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -Wall -Wextra -g")
endif()

set(SRC_DIR src)

find_package(Qt6 COMPONENTS Core Quick Qml QUIET)

if(Qt6_FOUND)
    message(STATUS "собираем GUI версию")

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    qt_add_resources(QML_RESOURCES src/ui/qml/qml.qrc)
    
    include_directories(
        ${SRC_DIR}
        ${SRC_DIR}/modules
        ${SRC_DIR}/modules/numbers
        ${SRC_DIR}/modules/numbers/natural
        ${SRC_DIR}/modules/numbers/integer
        ${SRC_DIR}/modules/numbers/rational
        ${SRC_DIR}/modules/polynomial
        ${SRC_DIR}/api
        ${SRC_DIR}/api/calculator
        ${SRC_DIR}/ui
    )
    
    file(GLOB SOURCES
        "${SRC_DIR}/main.cpp"
        "${SRC_DIR}/modules/numbers/natural/*.cpp"
        "${SRC_DIR}/modules/numbers/integer/*.cpp"
        "${SRC_DIR}/modules/numbers/rational/*.cpp"
        "${SRC_DIR}/modules/polynomial/*.cpp"
        "${SRC_DIR}/api/calculator/*.cpp"
        "${SRC_DIR}/ui/*.cpp"
    )
    
    add_executable(app ${SOURCES} ${QML_RESOURCES})
    target_compile_features(app PRIVATE cxx_std_20)
    target_compile_definitions(app PRIVATE USE_QT_GUI)
    target_link_libraries(app PRIVATE Qt6::Core Qt6::Quick Qt6::Qml)
    
else()
    message(STATUS "Qt6 не найден - собираем консольную версию")
    
    include_directories(
        ${SRC_DIR}
        ${SRC_DIR}/modules
        ${SRC_DIR}/modules/numbers
        ${SRC_DIR}/modules/numbers/natural
        ${SRC_DIR}/modules/numbers/integer
        ${SRC_DIR}/modules/numbers/rational
        ${SRC_DIR}/modules/polynomial
        ${SRC_DIR}/api
        ${SRC_DIR}/api/calculator
        ${SRC_DIR}/ui
    )
    
    file(GLOB SOURCES
        "${SRC_DIR}/*.cpp"
        "${SRC_DIR}/modules/numbers/natural/*.cpp"
        "${SRC_DIR}/modules/numbers/integer/*.cpp"
        "${SRC_DIR}/modules/numbers/rational/*.cpp"
        "${SRC_DIR}/modules/polynomial/*.cpp"
        "${SRC_DIR}/api/calculator/*.cpp"
    )
    
    add_executable(app ${SOURCES})
    target_compile_features(app PRIVATE cxx_std_20)
endif()

enable_testing()
# add_subdirectory(tst)
