#include "controllers.hpp"
#include <fstream>
#include <random>

std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk *)> sfxController::wavPtr(nullptr, &Mix_FreeChunk);
std::unique_ptr<Mix_Music, void(*)(Mix_Music *)> sfxController::sndPtr(nullptr, &Mix_FreeMusic);
int sfxController::lastPlayed = -1;

using namespace std::filesystem;

std::vector<path> sfxController::get() {
    if (!exists(path("sfxs")) || !is_directory("sfxs")) {
        SDL_Log("\"sfxs\" directory not found, closing program.");
        isRunning = false;
        return {};
    }
    std::vector<path> sfxFiles;
    for (const directory_entry &file : directory_iterator("sfxs")) {
        if (!file.is_directory()) {
            sfxFiles.push_back(file.path());
        }
    }
    return sfxFiles;
}

int sfxController::getChunkLength(Mix_Chunk *chunk, const char *path) {
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

int sfxController::playsfx() {
    const std::vector<path> sfxList = get();
    if (sfxList.size() == 0) {
        SDL_Log("\"sfxs\" directory is empty, closing program.");
        isRunning = false;
        return -1;
    }

    int index = RNG::intRange(sfxList.size());
    if (index == lastPlayed) {
        index = index + 1 >= sfxList.size() ? 0 : index + 1;
    }

    const path sfxPath = path(sfxList[index]);
    const path sfxExt = sfxPath.extension();
    lastPlayed = index;

    if (sfxExt == "wav") {
        wavPtr = std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk *)>(Mix_LoadWAV(sfxPath.string().c_str()), &Mix_FreeChunk);
        if (!wavPtr) {
            SDL_Log("File error: %s", Mix_GetError());
            return -1;
        }
        Mix_PlayChannel(-1, wavPtr.get(), 0);
        int length = getChunkLength(wavPtr.get(), sfxList[index].string().c_str());
        return length;
    }

    sndPtr = std::unique_ptr<Mix_Music, void(*)(Mix_Music *)>(Mix_LoadMUS(sfxPath.string().c_str()), &Mix_FreeMusic);
    if (!sndPtr) {
        SDL_Log("File error: %s", Mix_GetError());
        return -1;
    }
    Mix_PlayMusic(sndPtr.get(), 0);
    return 1000.0 * Mix_MusicDuration(sndPtr.get());
}

void sfxController::freePtrs() {
    Mix_FreeChunk(wavPtr.get());
    Mix_FreeMusic(sndPtr.get());
}