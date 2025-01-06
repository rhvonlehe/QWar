option(test-azmq "tests for azmq" OFF)
option(test-msgpack "tests for msgpack" OFF)

set(ENABLE_PRECOMPILED OFF CACHE INTERNAL "turn off precompiled headers (zeromq)")

include(FetchContent)
FetchContent_Declare(
    ZeroMQ
    GIT_REPOSITORY https://github.com/zeromq/libzmq
    GIT_TAG v4.3.4
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(ZeroMQ)

FetchContent_Declare(
    azmq
    GIT_REPOSITORY https://github.com/rhvonlehe/azmq
    GIT_TAG 9ce6f13a388e8072f4835c2fb238ede15c6cfa5b
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(azmq)

FetchContent_Declare(
    msgpackc-cxx
    GIT_REPOSITORY https://github.com/msgpack/msgpack-c
    GIT_TAG cpp-5.0.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(msgpackc-cxx)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS thread system REQUIRED)
find_package(Threads)
find_package(Qt6 COMPONENTS Core Gui Quick Qml  REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

#message("boost_FOUND is ${Boost_FOUND}")
#message("boost_include_dirs: ${Boost_INCLUDE_DIRS}")
#message("boost  libraries: ${Boost_LIBRARIES}")

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#  message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#execute_process(COMMAND "${CMAKE_COMMAND}" "-E" "environment")
