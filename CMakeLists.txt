cmake_minimum_required(VERSION 3.25)
project(Hyper8080 C CXX)

set(CMAKE_C_STANDARD 99)
find_package(SDL2 REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
        src/core/types.h

        src/emulation/CPU.cpp
        src/emulation/CPU.h
        src/emulation/instructions.h

        src/gui/Backend.cpp
        src/gui/Backend.h
        src/gui/Window.cpp
        src/gui/Window.h


        src/main.cpp
        src/cpu.cpp
        src/cpu.h
        src/instructions_C.cpp
        src/instructions_C.h
        src/debug.cpp
        src/debug.h
        src/render.cpp
        src/render.h
        src/sys.cpp
        src/sys.h
        src/defs.h
        src/gui/Window.cpp
        src/gui/Window.h
        src/gui/Backend.cpp src/gui/Backend.h src/emulation/CPU.cpp src/emulation/CPU.h src/core/types.h src/emulation/instructions.h)


target_link_libraries(${CMAKE_PROJECT_NAME} SDL2::SDL2)
target_link_libraries(${CMAKE_PROJECT_NAME} m)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "src/")