include(GoogleTest)

add_executable(PlayFullGame PlayFullGame.cpp)

target_link_libraries(PlayFullGame
    PRIVATE
    gameplay
)

gtest_add_tests(
    TARGET      PlayFullGame
    TEST_SUFFIX .noArgs
    TEST_LIST   noArgsTests
)



