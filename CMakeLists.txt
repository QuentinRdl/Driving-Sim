cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(src)

find_package(Curses REQUIRED) # For the key pressed.
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Driving_Sim
        src/main.cpp
        src/game.cpp
        src/circuit.cpp
        src/fpscounter.cpp
        src/fpscounter.h)

target_include_directories(Driving_Sim PRIVATE
    src
    src/GraphicsUtils
)


target_link_libraries(Driving_Sim ${CURSES_LIBRARIES})
target_link_libraries(Driving_Sim sfml-graphics sfml-audio)
