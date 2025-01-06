cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Driving_Sim
        src/utils.cpp
        src/utils.h
        src/car.cpp
        src/car.h
        src/surface.cpp
        src/surface.h
        src/main.cpp)
