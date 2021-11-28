find_package(Boost REQUIRED COMPONENTS system)

add_library(gameplay STATIC)

target_include_directories(gameplay
    PUBLIC
    .
    ${Boost_INCLUDE_DIR}
    )

target_sources(gameplay
    PRIVATE
    Card.cpp
    Deck.cpp
    Player.cpp
    PlayerState.cpp
    Round.cpp
    Game.cpp
    )

target_link_libraries(gameplay
    PRIVATE
    ${Boost_LIBRARIES}
    )
