cmake_minimum_required(VERSION 3.25)
project(rank)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(rank
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${SDL2IMAGE_INCLUDE_DIRS}
)

add_executable(rank 
    src/main.cpp 
    src/screen.cpp
)

target_link_libraries(rank 
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
)