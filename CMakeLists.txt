cmake_minimum_required(VERSION 3.25)
project(Hyper8080 C CXX)

set(CMAKE_C_STANDARD 99)
find_package(SDL2 REQUIRED)
find_package(spdlog REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
        src/Core/Types.hpp

        src/Emulation/CPU.cpp
        src/Emulation/CPU.hpp
        src/Emulation/Instructions.hpp

        src/GUI/Backend.cpp
        src/GUI/Backend.hpp
        src/GUI/Window.cpp
        src/GUI/Window.hpp

        src/MainC.cpp
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
        src/GUI/Window.cpp
        src/GUI/Window.hpp
        src/GUI/Backend.cpp
        src/GUI/Backend.hpp
        src/Emulation/CPU.cpp
        src/Emulation/CPU.hpp
        src/Core/Types.hpp
        src/Emulation/Instructions.hpp
        src/Main.cpp
        src/Core/Rendering.hpp)


target_link_libraries(Hyper8080 SDL2::SDL2)
target_link_libraries(Hyper8080 spdlog::spdlog)

target_include_directories(Hyper8080 PUBLIC "src/")