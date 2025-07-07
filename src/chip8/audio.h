#include <SDL2/SDL.h>
#include <iostream>

class Audio {
    public:
        Audio();
        ~Audio();
        
        void playAudio();
    private:
        SDL_AudioDeviceID device;
        SDL_AudioSpec wav;
        uint32_t wav_length;
        uint8_t *wav_buffer;

        bool audio_loaded = false;
};