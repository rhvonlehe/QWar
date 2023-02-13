
project(msgpack-test)

add_executable(msgpack-test MsgpackTest.cpp)

add_custom_command(TARGET msgpack-test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:libzmq> $<TARGET_FILE_DIR:msgpack-test>
)


target_link_libraries(msgpack-test
    PRIVATE
    msgpackc-cxx
    azmq
    Boost::boost
)
