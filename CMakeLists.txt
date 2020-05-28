cmake_minimum_required(VERSION 3.0)

project(QWar)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    Card.cpp
    Deck.cpp
    Player.cpp
    Round.cpp
    Game.cpp
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Core
    Qt5::Gui
    )
