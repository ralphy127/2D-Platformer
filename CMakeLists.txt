cmake_minimum_required(VERSION 3.10)
project(game)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED sdl2_image)
pkg_check_modules(SDL_TTF REQUIRED sdl2_ttf)
pkg_check_modules(SDL_MIXER REQUIRED sdl2_mixer)

include_directories(${PROJECT_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/Main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${SDL2_LIBRARIES} SDL2_image SDL2_ttf SDL2_mixer m
)

option(ENABLE_ERROR_TEST "Włącz test błędu kompilacji" OFF)
if(ENABLE_ERROR_TEST)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_ERROR_TEST)
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")