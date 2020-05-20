cmake_minimum_required(VERSION 3.10)

project(mur_ide)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS -std=c++17)

set(OpenCV_DIR C:/Libraries/LibOpenCV)


find_package(OpenCV REQUIRED)
find_package(Threads)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Widgets
        SerialPort
        WebSockets
        WebEngine
        PrintSupport
        Qml
        Quick
        Gui
        Gamepad
        Concurrent)

add_executable(${PROJECT_NAME} WIN32 resources/icon.rc)
#add_executable(${PROJECT_NAME} resources/icon.rc)

target_sources(${PROJECT_NAME}
        PRIVATE sources/main.cpp
        PRIVATE sources/Application.cpp
        PRIVATE sources/Application.hxx
        PRIVATE resources/resources.qrc
        PRIVATE sources/EditorController.cpp
        PRIVATE sources/EditorController.hxx
        PRIVATE sources/EditorErrors.cpp
        PRIVATE sources/EditorErrors.hxx
        PRIVATE sources/EditorHighlighter.cpp
        PRIVATE sources/EditorHighlighter.hxx
        PRIVATE sources/EditorSearch.cpp
        PRIVATE sources/EditorSearch.hxx
        PRIVATE sources/EditorSelection.cpp
        PRIVATE sources/EditorSelection.hxx
        PRIVATE sources/EditorUtils.cpp
        PRIVATE sources/EditorUtils.hxx
        PRIVATE sources/QmlUtils.cpp
        PRIVATE sources/QmlUtils.hxx
        PRIVATE sources/TextIO.cpp
        PRIVATE sources/TextIO.hxx
        PRIVATE sources/ApplicationController.cpp
        PRIVATE sources/ApplicationController.hxx
        PRIVATE sources/ApplicationMenu.cpp
        PRIVATE sources/ApplicationMenu.hxx
        PRIVATE sources/NetworkController.cpp
        PRIVATE sources/NetworkController.hxx
        PRIVATE sources/SimulatorController.cpp
        PRIVATE sources/SimulatorController.hxx
        PRIVATE sources/ApplicationLogger.cpp
        PRIVATE sources/ApplicationLogger.hxx
        PRIVATE sources/LocalScriptsController.hxx
        PRIVATE sources/JsonUtils.cpp
        PRIVATE sources/JsonUtils.hxx
        PRIVATE sources/RemoteController.cpp
        PRIVATE sources/RemoteController.hxx
        PRIVATE sources/QmlImageItem.cpp
        PRIVATE sources/QmlImageItem.hxx
        PRIVATE sources/Gamepad.cpp
        PRIVATE sources/Gamepad.hxx
        PRIVATE sources/EditorIndenter.cpp
        PRIVATE sources/EditorIndenter.hxx
        PRIVATE sources/SettingsController.cpp
        PRIVATE sources/SettingsController.hxx
        PRIVATE sources/UpdateController.cpp
        PRIVATE sources/UpdateController.hxx
        PRIVATE sources/EditorHints.cpp
        PRIVATE sources/EditorHints.hxx
        PRIVATE sources/HintBase.cpp
        PRIVATE sources/HintBase.hxx
        PRIVATE sources/ApiTokenDialog.cpp
        PRIVATE sources/ApiTokenDialog.hxx)

if (WIN32)
    target_sources(${PROJECT_NAME}
        PRIVATE sources/LocalScriptsController_win32.cpp)
else ()
    target_sources(${PROJECT_NAME}
        PRIVATE sources/LocalScriptsController_unix.cpp)
endif()

target_compile_features(${PROJECT_NAME}
        PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt5::SerialPort
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE Qt5::WebSockets
        PRIVATE Qt5::PrintSupport
        PRIVATE Qt5::WebEngine
        PRIVATE Qt5::Qml
        PRIVATE Qt5::Quick
        PRIVATE Qt5::Gui
        PRIVATE Qt5::Gamepad
        PRIVATE Qt5::Concurrent
        PRIVATE ${OpenCV_LIBS}
        ${CMAKE_THREAD_LIBS_INIT})

