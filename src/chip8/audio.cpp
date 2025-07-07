#include "audio.h"

Audio::Audio() {
    if (SDL_LoadWAV("src/chip8/beep.wav", &wav, &wav_buffer, &wav_length) == nullptr) {
        std::cerr << "Could not load audio: " << SDL_GetError() << std::endl;
        return;
    }

    device = SDL_OpenAudioDevice(nullptr, 0, &wav, nullptr, 0);
    if (device == 0) {
        std::cerr << "Could not load device: " << SDL_GetError() << std::endl;
        SDL_FreeWAV(wav_buffer);
        return;
    }

    audio_loaded = true;
}

Audio::~Audio() {
    if(!audio_loaded)
        return;

    SDL_CloseAudioDevice(device);
    SDL_FreeWAV(wav_buffer);
}

void Audio::playAudio() {
    if (!audio_loaded)
        return;
    
    if (SDL_GetQueuedAudioSize(device) == 0) {
        SDL_ClearQueuedAudio(device);
        SDL_QueueAudio(device, wav_buffer, wav_length);
        SDL_PauseAudioDevice(device, 0);
    }
}