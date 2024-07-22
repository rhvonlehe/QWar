
add_library(QWarServer STATIC
    StatePublisher.h StatePublisher.cpp
    StateReceiver.h StateReceiver.cpp)

target_include_directories(QWarServer PUBLIC .)

target_sources(QWarServer
    PRIVATE
    Server.cpp
)

target_link_libraries(QWarServer
    PRIVATE
    gameplay
    Boost::boost
    azmq
)
