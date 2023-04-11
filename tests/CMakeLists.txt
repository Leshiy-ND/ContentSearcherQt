cmake_minimum_required(VERSION 3.24)

project(ContentSearcherQt_tst VERSION 0.1 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(PROJECT_SOURCES
	"${CMAKE_SOURCE_DIR}/ConverterJSON.hpp"
	"${CMAKE_SOURCE_DIR}/ConverterJSON.cpp"
	"${CMAKE_SOURCE_DIR}/InvertedIndex.hpp"
	"${CMAKE_SOURCE_DIR}/InvertedIndex.cpp"
	"${CMAKE_SOURCE_DIR}/SearchServer.hpp"
	"${CMAKE_SOURCE_DIR}/SearchServer.cpp"
)

file(GLOB TESTS "${CMAKE_SOURCE_DIR}/tests/*.cpp")

add_executable(ContentSearcher_tst ${PROJECT_SOURCES} ${TESTS})

target_include_directories(ContentSearcher_tst PUBLIC
        "${CMAKE_SOURCE_DIR}"
        "${PROJECT_BINARY_DIR}/.."
)

target_link_libraries(ContentSearcher_tst PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(ContentSearcher_tst PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(ContentSearcher_tst)
