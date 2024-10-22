#pragma once
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_log.h"
#include <vector>
#include <string>
#include <memory>

class memeController{
    static std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk*)> wavPtr;
    static std::unique_ptr<Mix_Music, void(*)(Mix_Music*)> sndPtr;
    static int lastPlayed;
    static int getChunkLength(Mix_Chunk *chunk, const char *path);
public:
    static std::vector<std::string> get();
    static int playMeme();
};

class RNG{
public:
    static void setSeed(int seed);
    static float range();
    static int intRange(int max = RAND_MAX);
    static int intRange(int min, int max);
};