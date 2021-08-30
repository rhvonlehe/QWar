cmake_minimum_required(VERSION 3.17)

project(QWar)

add_subdirectory(gameplay)

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
    QGame.cpp
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
