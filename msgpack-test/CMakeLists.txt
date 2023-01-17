
project(msgpack-test)

add_executable(msgpack-test MsgpackTest.cpp)

target_link_libraries(msgpack-test
    PRIVATE
    msgpackc-cxx
    azmq
)
