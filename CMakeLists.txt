cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(src)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(Driving_Sim
        src/main.cpp
        src/game.cpp
        src/circuit.cpp
        src/fpscounter.cpp
        src/car.cpp
        src/debugmode.cpp
        src/arial_ttf.cpp
        src/vehicle.cpp
        src/Headers/plotting.h
        src/Headers/gnuplot-iostream.h
        src/plotting.cpp
)

target_include_directories(Driving_Sim PRIVATE

        src/Headers
        src/Headers/GraphicsUtils
        src/Headers/Physics
)

target_link_libraries(Driving_Sim sfml-graphics sfml-audio)

add_custom_command(TARGET Driving_Sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Driving_Sim>/assets)

# ------------------------------------------------------------------
# Ajout de l'exécutable de tests avec GoogleTest
# ------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        075196ca060107d44b4e3a1f44b25886ed5bd187 # v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

add_executable(DrivingSimTests
        src/vehicle.cpp
        tests/vehicle_test.cpp)

target_include_directories(DrivingSimTests PRIVATE
        src/Headers
        src/Headers/GraphicsUtils
        src/Headers/Physics
)

target_compile_options(DrivingSimTests PRIVATE "-Wall" "-Wextra" "-g" "-O0" "-fsanitize=address,undefined")
set_target_properties(DrivingSimTests PROPERTIES LINK_FLAGS "-fsanitize=address,undefined")

target_link_libraries(DrivingSimTests PRIVATE
        GTest::gtest_main
        Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(DrivingSimTests)
