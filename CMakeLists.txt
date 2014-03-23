cmake_minimum_required(VERSION 2.8)

project(texteditor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall")

aux_source_directory(. SOURCE_FILES)
add_executable(texteditor ${SOURCE_FILES})

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(texteditor ${SDL2_LIBRARIES})

pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
include_directories(${SDL2TTF_INCLUDE_DIRS})
target_link_libraries(texteditor ${SDL2TTF_LIBRARIES})
