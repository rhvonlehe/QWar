cmake_minimum_required(VERSION 3.17)

project(QWar)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS thread system REQUIRED)
find_package(Threads)

set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(_WIN32_WINDOWS=0x0a00)

add_subdirectory(gameplay)
add_subdirectory(networking)

set(CMAKE_AUTOMOC ON)
set(QML_IMPORT_PATH ${CMAKE_PREFIX_PATH}/qml CACHE PATH "qml import path")

find_package(Qt5 COMPONENTS Core Gui Quick Qml QuickCompiler REQUIRED)

add_executable(QWar)

target_include_directories(QWar PUBLIC  .)

qtquick_compiler_add_resources(RESOURCES resources.qrc)

target_sources(QWar
    PRIVATE
    ${RESOURCES}
    main.cpp
    QWarApplication.cpp
    QGameController.cpp
    QPlayer.cpp
    QGameModel.cpp
    )

target_link_libraries(QWar
    PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Quick
    Qt5::Qml
    gameplay
    )
