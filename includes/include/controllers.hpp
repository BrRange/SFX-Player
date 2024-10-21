#pragma once
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_log.h"
#include <vector>
#include <string>

class memeController{
    static Mix_Chunk *sound;
    static int lastPlayed;
    static int getChunkLength(Mix_Chunk *chunk, const char *path);
public:
    static std::vector<std::string> get();
    static int playMeme();
    ~memeController();
};

class RNG{
public:
    static void setSeed(int seed);
    static float range();
    static int intRange(int max = RAND_MAX);
    static int intRange(int min, int max);
};