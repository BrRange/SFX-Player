#include "controllers.hpp"
#include <fstream>
#include <random>
#include <filesystem>

Mix_Chunk *memeController::sound = nullptr;
int memeController::lastPlayed = -1;
std::vector<std::string> memeController::get(){
    std::vector<std::string> memeFiles;
    for(const std::filesystem::directory_entry& file : std::filesystem::directory_iterator("memes")){
        if(!file.is_directory()){
            memeFiles.push_back(file.path().string());
        }
    }
    return memeFiles;
}
int memeController::getChunkLength(Mix_Chunk *chunk, const char *path){
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    if (SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavLength) == NULL) {
        Mix_FreeChunk(chunk);
        return -1;
    }

    int lengthMs = (chunk->alen * 1000) / (wavSpec.freq * wavSpec.channels * (SDL_AUDIO_BITSIZE(wavSpec.format) / 8));

    SDL_FreeWAV(wavBuffer);
    return lengthMs;
}
int memeController::playMeme(){
    const std::vector<std::string> memeList = get();
    int index = RNG::intRange(memeList.size());
    if(index == lastPlayed){
        index = index + 1 >= memeList.size() ? 0 : index + 1;
    }
    sound = Mix_LoadWAV(memeList[index].c_str());
    if(!sound){
        SDL_Log("File error: %s", Mix_GetError());
        return -1;
    }
    Mix_PlayChannel(-1, sound, 0);
    lastPlayed = index;
    int length = getChunkLength(sound, memeList[index].c_str());
    return length;
}
memeController::~memeController(){
    Mix_FreeChunk(sound);
    SDL_Log("Exitting");
}

void RNG::setSeed(int seed){
    std::srand(seed);
}
float RNG::range(){
    return std::rand() / static_cast<float>(RAND_MAX);
}
int RNG::intRange(int max){
    return range() * max;
}
int RNG::intRange(int min, int max){
    return range() * (max - min) + min;
}