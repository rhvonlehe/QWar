
add_library(networking STATIC)

target_include_directories(networking PUBLIC .)

target_compile_definitions(networking PUBLIC BOOST_BIND_GLOBAL_PLACEHOLDERS)

target_sources(networking
    PRIVATE
    dummy.cpp
)

target_link_libraries(networking
    PUBLIC
    Boost::boost
)
