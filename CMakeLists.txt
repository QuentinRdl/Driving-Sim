cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)

# Include the source directory
include_directories(src)

# Include Eigen
include_directories(/usr/include/eigen3)

# Include matplotlibcpp (update the path to where matplotlibcpp is located)
# TODO : How to make same for every user ??
# include_directories(/home/keiris/matplotlib-cpp)

# Find and link the Curses library (used for key input)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Find Python3 and its components (for matplotlibcpp)
find_package(Python3 COMPONENTS Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

# Add the executable and all source files
add_executable(Driving_Sim
        src/utils.cpp
        src/utils.h
        src/car.cpp
        src/car.h
        src/surface.cpp
        src/surface.h
        src/mainUnusedForNow.cpp
        src/VehiculeDynamics.cpp
        src/VehiculeDynamics.h
        src/matplotlib_test.cpp
        src/matplotlibcpp.h)

# Link libraries
target_link_libraries(Driving_Sim ${CURSES_LIBRARIES} ${Python3_LIBRARIES})
