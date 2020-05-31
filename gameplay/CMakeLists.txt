

add_library(gameplay STATIC)

target_include_directories(gameplay PUBLIC .)

target_sources(gameplay
    PRIVATE
    Card.cpp
    Deck.cpp
    Player.cpp
    Round.cpp
    Game.cpp
    )
