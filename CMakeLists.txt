cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

find_package(Curses REQUIRED) # For the key pressed.
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Driving_Sim
        src/utils.cpp
        src/utils.h
        src/car.cpp
        src/car.h
        src/surface.cpp
        src/surface.h
        src/main.cpp)

target_link_libraries(Driving_Sim ${CURSES_LIBRARIES})
target_link_libraries(Driving_Sim sfml-graphics sfml-audio)
