cmake_minimum_required(VERSION 3.25)
project(ppd_2023_seq C)

set(CMAKE_C_STANDARD 23)

add_executable(ppd_2023_seq main.c structures/grafo.h structures/grafo.c graphics/sdl_utils.c graphics/sdl_utils.h)
target_link_libraries(ppd_2023_seq -lSDL2 -lSDL2_ttf m)
