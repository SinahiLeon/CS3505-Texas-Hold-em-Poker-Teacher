cmake_minimum_required(VERSION 3.16)

project(PokerTests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PokerTests tst_cardhandtests.cpp
    tst_lessontests.cpp

)
add_test(NAME PokerTests COMMAND PokerTests)

target_link_libraries(PokerTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(PokerTests PRIVATE PokerCore)
