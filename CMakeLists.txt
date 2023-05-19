cmake_minimum_required(VERSION 3.25)
project(Hyper8080 C CXX)

set(CMAKE_C_STANDARD 99)
find_package(SDL2 REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
        src/main.cpp
        src/cpu.cpp
        src/cpu.h
        src/instructions.cpp
        src/instructions.h
        src/debug.cpp
        src/debug.h
        src/render.cpp
        src/render.h
        src/sys.cpp
        src/sys.h
        src/defs.h
        )

target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2)
target_link_libraries(${CMAKE_PROJECT_NAME} m)