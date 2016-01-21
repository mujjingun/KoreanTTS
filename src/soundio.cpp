#include <cstdio>
#include <algorithm>
#include <cstdint>

#include "soundio.h"
#include "main.h"

int SoundIO::init_sound(int channels)
{
    this->channels = channels;

    PaStreamParameters outputParameters;
    PaError err;

    // initialize audio
    err = Pa_Initialize();
    if( err != paNoError ) return 1;
    puts("Initialized");

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

    outputParameters.channelCount = channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              Fs,
              FRAMES_PER_BUFFER,
              paClipOff,
              NULL,
              NULL );
    if( err != paNoError ) return 1;

    err = Pa_StartStream( stream );
    if( err != paNoError ) return 1;
    puts("Stream Opened");


    //////////////////////////

    SF_INFO sfinfo;
    sfinfo.channels = channels;
    sfinfo.samplerate = Fs;
    //sfinfo.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    file = sf_open("sound.wav", SFM_WRITE, &sfinfo);

    puts("File Opened");

    return 0;
}

int SoundIO::play_sound(std::vector<float> data)
{
    PaError err;
    int floats_per_frame = FRAMES_PER_BUFFER * channels;
    for(unsigned i = 0; i < data.size(); i += floats_per_frame)
    {
        err = Pa_WriteStream( stream, &data[i], FRAMES_PER_BUFFER );
        if( err != paNoError ) return 1;
    }

    sf_write_float(file, &data[0], data.size());

    return 0;
}

int SoundIO::end_sound()
{
    PaError err;
    err = Pa_StopStream( stream );
    if( err != paNoError ) return 1;
    puts("Stream Stopped");

    Pa_Terminate();

    sf_write_sync(file);
    sf_close(file);
    puts("File Closed");

    return 0;
}

int16_t SoundIO::normalize(const fpoint& x)
{
    return std::min(std::max(int(x * INT16_MAX), INT16_MIN), INT16_MAX);
}
