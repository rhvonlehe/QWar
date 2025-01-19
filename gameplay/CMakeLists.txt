get_property(propVal TARGET Boost::boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "Boost::boost include dirs: ${propVal}")


# Define target
#
add_library(gameplay STATIC
    EventSchedulerDefs.h)

target_include_directories(gameplay
    PUBLIC
    .
    )

target_sources(gameplay
    PRIVATE
    Card.cpp
    Deck.cpp
    EventScheduler.h
    Game.cpp
    Player.cpp
    PlayerEvents.h
    PlayerState.cpp
    Round.cpp
    RoundState.cpp
    TimerService.cpp
    )

target_compile_definitions(gameplay PUBLIC BOOST_BIND_GLOBAL_PLACEHOLDERS)

target_link_libraries(gameplay
    PUBLIC
    Boost::boost
    Boost::system
    Boost::thread
    )

# Testing
#
if (GAMEPLAY_ENABLE_TESTING OR PROJECT_IS_TOP_LEVEL)
    add_subdirectory(test)

endif()
