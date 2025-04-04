cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


find_package(SFML REQUIRED COMPONENTS graphics audio)
find_package(Boost REQUIRED COMPONENTS iostreams)
find_package(GTest REQUIRED)
find_package(Threads)


# ------------------------------------------------------------------
# Main Build
# ------------------------------------------------------------------

add_executable(Driving_Sim
        src/main.cpp
        src/game.cpp
        src/circuit.cpp
        src/fpscounter.cpp
        src/car.cpp
        src/debugmode.cpp
        src/dejavu_sans_mono_ttf.cpp
        src/vehicle.cpp
        src/plotting.cpp
)

target_include_directories(Driving_Sim PRIVATE
        src
        src/Headers
        src/Headers/GraphicsUtils
        src/Headers/Physics
)

target_link_libraries(Driving_Sim
        sfml-graphics
        sfml-audio
        Boost::iostreams
)

# Copie des assets dans le dossier de build
add_custom_command(TARGET Driving_Sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Driving_Sim>/assets)

# ------------------------------------------------------------------
# Tests Build
# ------------------------------------------------------------------

enable_testing()

add_executable(DrivingSimTests
        src/vehicle.cpp
        src/plotting.cpp
        tests/vehicle_test.cpp
)

target_include_directories(DrivingSimTests PRIVATE
        src/Headers
        src/Headers/GraphicsUtils
        src/Headers/Physics
)

target_compile_options(DrivingSimTests PRIVATE
        -Wall -Wextra -g -O0 -fsanitize=address,undefined)

set_target_properties(DrivingSimTests PROPERTIES
        LINK_FLAGS "-fsanitize=address,undefined")

target_link_libraries(DrivingSimTests PRIVATE
        GTest::gtest
        Threads::Threads
        Boost::iostreams
)

include(GoogleTest)
gtest_discover_tests(DrivingSimTests)

