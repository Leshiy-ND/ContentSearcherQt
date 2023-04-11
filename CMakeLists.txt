cmake_minimum_required(VERSION 3.5)

project(ContentSearcherQt VERSION 0.1 LANGUAGES CXX)
configure_file(VersionConfig.h.in VersionConfig.h)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS -pthread)

find_package(QT NAMES Qt6 Qt5      REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include(FetchContent)
FetchContent_Declare(
	json
	URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

set(PROJECT_SOURCES
	ConverterJSON.hpp   ConverterJSON.cpp
	InvertedIndex.hpp   InvertedIndex.cpp
	 SearchServer.hpp    SearchServer.cpp
	   mainwindow.h        mainwindow.cpp   mainwindow.ui
	main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ContentSearcherQt
	    MANUAL_FINALIZATION
	    ${PROJECT_SOURCES}
    )
else()
    add_executable(ContentSearcherQt
	    ${PROJECT_SOURCES}
    )
endif()

target_include_directories(ContentSearcherQt PUBLIC
	"${PROJECT_BINARY_DIR}"
)

target_link_libraries(ContentSearcherQt PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(ContentSearcherQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(ContentSearcherQt PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

install(TARGETS ContentSearcherQt
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ContentSearcherQt)
endif()

#add_subdirectory(test)
#target_compile_definitions(ContentSearcherQt PRIVATE CONTENTSEARCHERQT_LIBRARY)
