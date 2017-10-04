cmake_minimum_required(VERSION 2.8.12)

project(QWar)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

add_executable(${PROJECT_NAME}
    main.cpp
    Card.cpp
    Deck.cpp
    Player.cpp
    Round.cpp
    Game.cpp)

target_link_libraries(${PROJECT_NAME} Qt5::Core)
