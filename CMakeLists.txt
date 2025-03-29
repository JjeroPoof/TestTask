cmake_minimum_required(VERSION 3.16)

project(TestTask VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# === Source files ===
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

file(GLOB_RECURSE HEADERS "${SRC_DIR}/*.h")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# === Target creation ===
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TestTask
        MANUAL_FINALIZATION
        ${HEADERS}
        ${SOURCES}
        ${RESOURCES}
    )
else()
    if(ANDROID)
        add_library(TestTask SHARED ${HEADERS} ${SOURCES} ${RESOURCES})
    else()
        add_executable(TestTask ${HEADERS} ${SOURCES} ${RESOURCES})
    endif()
endif()

# === Link libraries ===
target_link_libraries(TestTask PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# === Compiler definitions or flags if needed ===
target_compile_definitions(TestTask PRIVATE QT_DEPRECATED_WARNINGS)

# === MacOS/iOS specific bundle settings ===
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TestTask)
endif()

set_target_properties(TestTask PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# === Install rules ===
include(GNUInstallDirs)
install(TARGETS TestTask
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# === Finalize for Qt6 ===
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TestTask)
endif()
