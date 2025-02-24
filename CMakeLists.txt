cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(/usr/include/eigen3)

find_package(Python3 COMPONENTS Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

find_package(Boost COMPONENTS iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(MATPLOTLIBCPP_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/matplotlibcpp.h")
if(NOT EXISTS ${MATPLOTLIBCPP_HEADER})
    message(FATAL_ERROR "matplotlibcpp.h not found at ${MATPLOTLIBCPP_HEADER}. Please ensure it exists.")
endif()

# Définir les sources et headers (en adaptant aux fichiers réellement présents)
set(SOURCES
        src/TheLastOneISwear.cpp
)

set(HEADERS
        src/matplotlibcpp.h
)

# Création de l'exécutable
add_executable(Driving_Sim ${SOURCES} ${HEADERS})

# Lier les bibliothèques Python3 et Boost.iostreams
target_link_libraries(Driving_Sim ${Python3_LIBRARIES} Boost::iostreams)
