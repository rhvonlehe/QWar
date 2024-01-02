
add_executable(QWarServer)

target_sources(QWarServer
    PRIVATE
    main.cpp
)

target_link_libraries(QWarServer
    PRIVATE
    gameplay
    Boost::boost
    azmq
)
