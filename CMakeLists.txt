cmake_minimum_required(VERSION 3.10)

project(mur_ide)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS -std=c++17)

find_package(OpenCV REQUIRED)
find_package(Threads)

if (WIN32)
    find_package(glfw3 3.4 REQUIRED)
else()
    find_package(glfw3 3.3 REQUIRED)
endif()

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Widgets
        WebSockets
        WebEngineQuick
        PrintSupport
        Qml
        Quick
        QuickControls2
        Gui
        Concurrent)


add_executable(${PROJECT_NAME} WIN32 resources/icon.rc)

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
        PRIVATE sources/ApiTokenDialog.hxx
        PRIVATE sources/Joystick.cpp
        PRIVATE sources/Joystick.hxx
        PRIVATE sources/Keyboard.cpp
        PRIVATE sources/Keyboard.hxx
    )

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
        PRIVATE Qt6::Core
        PRIVATE Qt6::Widgets
        PRIVATE Qt6::WebSockets
        PRIVATE Qt6::PrintSupport
        PRIVATE Qt6::WebEngineQuick
        PRIVATE Qt6::Qml
        PRIVATE Qt6::Quick
        PRIVATE Qt6::QuickControls2
        PRIVATE Qt6::Gui
        PRIVATE Qt6::Concurrent
        PRIVATE ${OpenCV_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        glfw
    )

if (NOT WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()
