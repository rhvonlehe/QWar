
add_library(networking INTERFACE)

target_include_directories(networking INTERFACE .)

target_compile_definitions(gameplay PUBLIC BOOST_BIND_GLOBAL_PLACEHOLDERS)

target_link_libraries(networking
    INTERFACE
    ${Boost_LIBRARIES}
)
