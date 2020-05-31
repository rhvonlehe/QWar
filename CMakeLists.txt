cmake_minimum_required(VERSION 3.17)

project(QWar)

add_subdirectory(gameplay)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Core
    Qt5::Gui
    gameplay
    )
