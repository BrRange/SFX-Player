#pragma once
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_log.h"
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

extern bool isRunning;

class sfxController {
    static std::unique_ptr<Mix_Chunk, void (*)(Mix_Chunk*)> wavPtr;
    static std::unique_ptr<Mix_Music, void (*)(Mix_Music*)> sndPtr;
    static int lastPlayed;
    static int getChunkLength(Mix_Chunk* chunk, const char* path);

public:
    static std::vector<std::filesystem::path> get();
    static int playsfx();
};

namespace RNG {
    inline void setSeed(int seed) {
        std::srand(seed);
    }
    inline float range() {
        return std::rand() / static_cast<float>(RAND_MAX);
    }
    inline int intRange(int max) {
        return range() * max;
    }
    inline int intRange(int min, int max) {
        return range() * (max - min) + min;
    }
};