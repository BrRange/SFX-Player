#include <windows.h>
#include <thread>
#include <chrono>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "controllers.hpp"

#ifdef DEBUG
constinit static size_t memoryCount = 0ull;
void* operator new(size_t bytes) {
    SDL_Log("Allocating %i bytes", bytes);
    memoryCount += bytes;
    SDL_Log("Current memory in heap: %i", memoryCount);
    return malloc(bytes);
}
void operator delete(void* memory, size_t bytes) {
    SDL_Log("Deallocating %i bytes", bytes);
    memoryCount -= bytes;
    free(memory);
    SDL_Log("Heap memory left: %i", memoryCount);
}
#endif

void (*handleEvents)();
void (*sfxTime)();
bool isRunning = true;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (SDL_Init(SDL_INIT_AUDIO)) {
        SDL_Log("SDL error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (!Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3)) {
        SDL_Log("Mixer error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)) {
        SDL_Log("Audio error: %s", SDL_GetError());
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    using namespace std::chrono;

    RNG::setSeed(high_resolution_clock::now().time_since_epoch().count());
    static steady_clock::time_point cycleStart(steady_clock::now()), cycleEnd, timeSincesfx(cycleStart);
    static SDL_Event event;
    static size_t waitTime = RNG::intRange(RAND_MAX) / 10;

    handleEvents = [] {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                return;
            }
        }
    };
    sfxTime = [] {
        if (steady_clock::now() - timeSincesfx >= milliseconds(waitTime)) {
            size_t checkNum = 10ull * RNG::intRange(RAND_MAX);
            waitTime = sfxController::playsfx() + checkNum;
            timeSincesfx = steady_clock::now();
        }
    };

    while (isRunning) {
        cycleEnd = steady_clock::now();
        if ((cycleEnd - cycleStart).count() >= 10000000) {
            std::thread eventThread(handleEvents);
            cycleStart = cycleEnd;
            sfxTime();
            eventThread.join();
        }
        std::this_thread::sleep_for(1ms);
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    return 0;
}