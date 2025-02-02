cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)

# Include the source directory
include_directories(src)

# Include Eigen
include_directories(/usr/include/eigen3)

# Find Python3 and its components (for matplotlibcpp)
find_package(Python3 COMPONENTS Development NumPy REQUIRED)

# Include matplotlibcpp
include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

# Check if matplotlibcpp.h is available // TODO : For future ? => Update this if the header is stored in a specific location
set(MATPLOTLIBCPP_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/matplotlibcpp.h")
if(NOT EXISTS ${MATPLOTLIBCPP_HEADER})
    message(FATAL_ERROR "matplotlibcpp.h not found at ${MATPLOTLIBCPP_HEADER}. Please ensure it exists.")
endif()

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
        src/matplotlibcpp.h
        src/VehiculeDynamics2.cpp
        src/VehiculeDynamics2.h
)

# Link Python libraries
target_link_libraries(Driving_Sim ${Python3_LIBRARIES})