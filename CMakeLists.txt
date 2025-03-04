cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Répertoires d'inclusion
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(/usr/include/eigen3)

# Configuration de Python3
find_package(Python3 COMPONENTS Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

# Configuration de Boost
find_package(Boost COMPONENTS iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Vérification de la présence de matplotlibcpp.h dans src
set(MATPLOTLIBCPP_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/matplotlibcpp.h")
if(NOT EXISTS ${MATPLOTLIBCPP_HEADER})
    message(FATAL_ERROR "matplotlibcpp.h not found at ${MATPLOTLIBCPP_HEADER}. Please ensure it exists.")
endif()

# Sources et headers de l'exécutable principal
set(SOURCES
        src/VehiculeDynamics.cpp
)

set(HEADERS
        src/matplotlibcpp.h
)

# Création de l'exécutable principal
add_executable(Driving_Sim ${SOURCES} ${HEADERS})

# Lier les bibliothèques Python3 et Boost.iostreams
target_link_libraries(Driving_Sim ${Python3_LIBRARIES} Boost::iostreams)

# Commandes pour créer le dossier Images et ses sous-dossiers après compilation
add_custom_command(
        TARGET Driving_Sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Driving_Sim>/Images
)

add_custom_command(
        TARGET Driving_Sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Driving_Sim>/Images/Etape2
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Driving_Sim>/Images/Etape3
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Driving_Sim>/Images/Etape4
)

# ------------------------------------------------------------------
# Configuration des tests avec GoogleTest et téléchargement automatique
# ------------------------------------------------------------------
enable_testing()
find_package(Threads REQUIRED)

# Auto téléchargement de GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        075196ca060107d44b4e3a1f44b25886ed5bd187 # v1.15.2
)
FetchContent_MakeAvailable(googletest)


# Création de l'exécutable de test à partir de rk4_integration_test.cpp
add_executable(rk4_integration_test src/rk4_integration_test.cpp)



# Options de compilation pour le test (affichage des warnings, debug et sanitizers)
target_compile_options(rk4_integration_test
        PRIVATE "-Wall" "-Wextra" "-g" "-O0" "-fsanitize=address,undefined"
)

# Propriétés de linkage pour les sanitizers
set_target_properties(rk4_integration_test
        PROPERTIES LINK_FLAGS "-fsanitize=address,undefined"
)

# Lier GoogleTest et les threads
target_link_libraries(rk4_integration_test
        PRIVATE GTest::gtest_main Threads::Threads
)

# Découverte des tests via GoogleTest
include(GoogleTest)
gtest_discover_tests(rk4_integration_test)
