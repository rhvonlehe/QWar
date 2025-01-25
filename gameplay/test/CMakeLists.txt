include(GoogleTest)

add_executable(PlayFullGame
    PlayFullGame.cpp
    TestPlayer.h
    TestPlayer.cpp
)

target_include_directories(PlayFullGame PUBLIC .)

target_link_libraries(PlayFullGame
    PRIVATE
    gameplay
    GTest::gtest_main
)

gtest_add_tests(
    TARGET      PlayFullGame
    TEST_SUFFIX .noArgs
    TEST_LIST   noArgsTests
)


