

project(azmq-test)

add_executable(TestPublisher TestPublisher.cpp)

target_link_libraries(TestPublisher
    PRIVATE
    Boost::boost
    azmq
)

add_executable(TestSubscriber TestSubscriber.cpp)

target_link_libraries(TestSubscriber
    PRIVATE
    Boost::boost
    azmq
)

add_custom_command(TARGET TestSubscriber POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:libzmq> $<TARGET_FILE_DIR:TestSubscriber>
)


add_executable(nasdaq nasdaq.cpp)
#install(IMPORTED_RUNTIME_ARTIFACTS nasdaq )
target_link_libraries(nasdaq
    PRIVATE
    Boost::boost
    azmq
)

add_custom_command(TARGET nasdaq POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:libzmq> $<TARGET_FILE_DIR:nasdaq>
)
