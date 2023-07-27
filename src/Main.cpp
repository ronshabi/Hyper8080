#include <Emulation/CPU.hpp>

#include <GUI/Backend.hpp>
#include <GUI/Window.hpp>

#include <chrono>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>


#include <iostream>
#include <thread>

void PrintUsage()
{
    std::cerr << "usage: Hyper8080 <file>\n";
}

int main(int argc, const char* argv[])
{
    if (argc != 2) {
        PrintUsage();
        std::exit(1);
    }

    std::string programPath(argv[1]);
    spdlog::set_level(spdlog::level::debug);

    // Load program
    CPU cpu;

    try {
        cpu.LoadProgram(programPath);
    } catch (const std::runtime_error& rte) {
        spdlog::error("{}", rte.what());
        std::exit(1);
    }

    // Initialize backend
    GUI::Backend backend;
    SDL_Event e;

    GUI::Window mainWindow("Hyper8080");

    while (mainWindow.isRunning()) {
        // If SDL_PollEvent returns zero, there are no events left on queue
        if (SDL_PollEvent(&e)) {
            if (e.key.keysym.sym == SDLK_q) {
                spdlog::debug("Main thread: quit requested by pressing q key");
                mainWindow.Stop();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}