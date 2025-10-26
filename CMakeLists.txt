cmake_minimum_required(VERSION 3.12)
project(Algebra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -Wall -Wextra")

set(SRC_DIR src)

include_directories(
    ${SRC_DIR}
    ${SRC_DIR}/modules
    ${SRC_DIR}/modules/numbers
    ${SRC_DIR}/modules/numbers/natural
    ${SRC_DIR}/modules/numbers/integer
    ${SRC_DIR}/modules/numbers/rational
    ${SRC_DIR}/modules/polynomial
    ${SRC_DIR}/user_appearance
)

file(GLOB SOURCES
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/modules/*.cpp"
    "${SRC_DIR}/modules/numbers/*.cpp"
    "${SRC_DIR}/modules/numbers/natural/*.cpp"
    "${SRC_DIR}/modules/numbers/integer/*.cpp"
    "${SRC_DIR}/modules/numbers/rational/*.cpp"
    "${SRC_DIR}/modules/polynomial/*.cpp"
    "${SRC_DIR}/user_appearance/*.cpp"
)

add_executable(app ${SOURCES})
target_compile_features(app PRIVATE cxx_std_20)