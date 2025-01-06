# Preamble
cmake_minimum_required(VERSION 3.19)
project(QWar)

enable_testing()

# Project-wide setup
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(_WIN32_WINDOWS=0x0a00)

# Externally provided content
include(dependencies.cmake)


add_subdirectory(gameplay)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(test)

set(CMAKE_AUTOMOC ON)
set(QML_IMPORT_PATH ${CMAKE_PREFIX_PATH}/qml CACHE PATH "qml import path")


if(0)

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
endif()
