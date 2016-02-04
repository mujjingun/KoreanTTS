#ifndef SOUNDIO_H_INCLUDED
#define SOUNDIO_H_INCLUDED

#include <portaudio.h>
#include <sndfile.h>

#include <string>
#include "main.h"

class SoundIO
{
private:

    const int FRAMES_PER_BUFFER = 1024;

    PaStream *stream;

    SNDFILE *file;

    int channels;

public:

    SoundIO()
    : stream(nullptr), file(nullptr), channels(1)
    {};

    SoundIO(const SoundIO&) = delete;
    SoundIO operator=(const SoundIO&) = delete;

    int init_sound(int channels);
    int play_sound(std::vector<float> data);
    int end_sound();

};

signal_t get_samples_from_file(std::string filename);

#endif // SOUNDIO_H_INCLUDED
