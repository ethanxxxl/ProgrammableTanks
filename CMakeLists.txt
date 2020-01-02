cmake_minimum_required(VERSION 3.10)

project(Project C)

set(SRC
	src/main.c
	src/tank.c
	src/handle_input.c
	)

include_directories(./include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

add_executable(run ${SRC})
set_property(TARGET run PROPERTY C_STANDARD 11)

target_link_libraries(run m SDL2)
