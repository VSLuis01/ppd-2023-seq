cmake_minimum_required(VERSION 3.25)
project(ppd_2023_seq C)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(CMAKE_C_STANDARD 23)

add_executable(ppd_2023_seq main.c structures/grafo.h structures/grafo.c graphics/sdl_utils.c graphics/sdl_utils.h)
target_link_libraries(ppd_2023_seq ${SDL2_LIBRARIES} m)
