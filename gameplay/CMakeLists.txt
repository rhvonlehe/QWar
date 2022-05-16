
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
    RoundState.cpp
    Game.cpp
    TimerService.cpp
    )

target_compile_definitions(gameplay PUBLIC BOOST_BIND_GLOBAL_PLACEHOLDERS)

target_link_libraries(gameplay
    PRIVATE
    ${Boost_LIBRARIES}
    )
