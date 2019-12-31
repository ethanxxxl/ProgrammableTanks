cmake_minimum_required(VERSION 3.10)

project(Project C)

set(SRC
	src/main.c
	)

include_directories(./include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

add_executable(run ${SRC})

target_link_libraries(run)
