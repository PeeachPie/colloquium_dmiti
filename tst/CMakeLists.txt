include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)

file(GLOB APP_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/modules/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/modules/numbers/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/modules/numbers/natural/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/modules/numbers/integer/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/modules/numbers/rational/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/modules/polynomial/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/modules/calculator/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/api/calculator/*.cpp"
)

list(REMOVE_ITEM APP_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable(tests ${TEST_SOURCES} ${APP_SOURCES})

if(MSVC)
    target_compile_options(tests PRIVATE /FS)
endif()

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/modules
    ${CMAKE_SOURCE_DIR}/src/modules/numbers
    ${CMAKE_SOURCE_DIR}/src/modules/numbers/natural
    ${CMAKE_SOURCE_DIR}/src/modules/numbers/integer
    ${CMAKE_SOURCE_DIR}/src/modules/numbers/rational
    ${CMAKE_SOURCE_DIR}/src/modules/polynomial
    ${CMAKE_SOURCE_DIR}/src/modules/calculator
    ${CMAKE_SOURCE_DIR}/src/api/calculator
)

if(WIN32)
    target_link_libraries(tests PRIVATE gtest gtest_main)
else()
    target_link_libraries(tests PRIVATE gtest gtest_main pthread)
endif()

include(GoogleTest)
gtest_discover_tests(tests)
