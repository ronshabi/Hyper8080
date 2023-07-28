#include <Emulation/CPU.hpp>

#include <GUI/Backend.hpp>
#include <GUI/Window.hpp>

#include <SDL_events.h>
#include <SDL_keycode.h>
#include <chrono>
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

    GUI::Window mainWindow("Hyper8080", 224, 256);
    mainWindow.SetBitmapSource(cpu.GetMemoryAtOffset(0));
    mainWindow.setDrawFromBitmapSource(true);

    u8 interrupt_number = 1;

    while (mainWindow.isRunning()) {
        // If SDL_PollEvent returns zero, there are no events left on queue
        SDL_PollEvent(&e);

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_q) {
                spdlog::debug("Main thread: quit requested by pressing q key");
                mainWindow.Stop();
            } else if (e.key.keysym.sym == SDLK_DOWN) {
                spdlog::debug("[DOWN] pressed");
            } else if (e.key.keysym.sym == SDLK_LEFT) {
                spdlog::debug("[LEFT] pressed");
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                spdlog::debug("[RIGHT] pressed");
            } else if (e.key.keysym.sym == SDLK_UP) {
                spdlog::debug("[UP] pressed");
            }
        } else if (e.type == SDL_QUIT) {
            spdlog::debug("Main thread: quit requested");
            mainWindow.Stop();
        }

        SDL_FlushEvents(0, UINT32_MAX);

        // CPU LOOP
        cpu.Execute();

        std::this_thread::sleep_for(std::chrono::milliseconds(8));
        cpu.Interrupt(interrupt_number + 1);
        interrupt_number ^= 1;
    }
}