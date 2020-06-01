cmake_minimum_required(VERSION 3.17)

project(QWar)

add_subdirectory(gameplay)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_executable(QWar)

target_include_directories(QWar PUBLIC  .)

target_sources(QWar
    PRIVATE
    main.cpp
    QWarApplication.cpp
    )

target_link_libraries(QWar
    PRIVATE
    Qt5::Core
    Qt5::Gui
    gameplay
    )
