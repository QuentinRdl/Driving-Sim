cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(src)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# Inclusion de Boost pour les graphes
find_package(Boost COMPONENTS iostreams REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(Driving_Sim
        src/main.cpp
        src/game.cpp
        src/circuit.cpp
        src/fpscounter.cpp
        src/car.cpp
        src/debugmode.cpp
        src/dejavu_sans_mono_ttf.cpp
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

target_link_libraries(Driving_Sim
        sfml-graphics
        sfml-audio
        ${Boost_LIBRARIES}
)

add_custom_command(TARGET Driving_Sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Driving_Sim>/assets)

# ------------------------------------------------------------------
# Ajout de l'exécutable de tests avec GoogleTest
# ------------------------------------------------------------------

include(FetchContent )
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        075196ca060107d44b4e3a1f44b25886ed5bd187 # v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

add_executable(DrivingSim_Tests
        src/vehicle.cpp
        src/plotting.cpp
        tests/vehicle_test.cpp)

target_include_directories(DrivingSim_Tests PRIVATE
        src/Headers
        src/Headers/GraphicsUtils
        src/Headers/Physics
)

target_compile_options(DrivingSim_Tests PRIVATE "-Wall" "-Wextra" "-g" "-O0" "-fsanitize=address,undefined")
set_target_properties(DrivingSim_Tests PROPERTIES LINK_FLAGS "-fsanitize=address,undefined")

target_link_libraries(DrivingSim_Tests PRIVATE
        GTest::gtest_main
        Threads::Threads
        ${Boost_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(DrivingSim_Tests)
