cmake_minimum_required(VERSION 3.21)

project(SoLong LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(so_long
    src/main.c
    src/map.c
	src/game.c
	src/render.c
	test/test_game.c
)

target_compile_definitions(so_long PRIVATE
    _POSIX_C_SOURCE=200809L
)

target_link_libraries(so_long PRIVATE raylib)
