cmake_minimum_required(VERSION 3.5)

project(TestInvertedIndex LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5      REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(TestInvertedIndex test_InvertedIndex.cpp)
target_link_libraries(TestInvertedIndex PRIVATE ContentSearcherQt)
add_test(NAME TestInvertedIndex COMMAND TestInvertedIndex)

target_link_libraries(TestInvertedIndex PRIVATE Qt${QT_VERSION_MAJOR}::Test)
