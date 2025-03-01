cmake_minimum_required(VERSION 3.30)
project(Driving_Sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(src)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
if (NOT SFML_FOUND)
    if (EXISTS "/etc/arch-release")
        execute_process(COMMAND sudo pacman -S sfml --noconfirm)
    else()
        execute_process(COMMAND sudo apt-get install libsfml-dev -y)
    endif()
endif()


add_executable(Driving_Sim
        src/main.cpp
        src/game.cpp
        src/circuit.cpp
        src/fpscounter.cpp
        src/car.cpp
)

target_include_directories(Driving_Sim PRIVATE
    src/Headers
    src/Headers/GraphicsUtils
    src/Headers/Physics
)

target_link_libraries(Driving_Sim sfml-graphics sfml-audio)

# Copier les assets dans le dossier de sortie
add_custom_command(TARGET Driving_Sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Driving_Sim>/assets)