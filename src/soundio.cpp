#include <iostream>
#include <algorithm>
#include <cstdint>

#include "soundio.h"
#include "main.h"

int SoundIO::init_sound(int achannels)
{
    this->channels = achannels;

    PaStreamParameters outputParameters;
    PaError err;

    // initialize audio
    err = Pa_Initialize();
    if( err != paNoError ) return 1;

    std::cout << "Initialized Portaudio." << std::endl;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

    outputParameters.channelCount = channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    err = Pa_OpenStream(
              &stream,
              nullptr, /* no input */
              &outputParameters,
              Fs,
              FRAMES_PER_BUFFER,
              paClipOff,
              nullptr,
              nullptr );
    if( err != paNoError ) return 1;

    err = Pa_StartStream( stream );
    if( err != paNoError ) return 1;
    std::cout << "Stream Opened" << std::endl;


    //////////////////////////

    SF_INFO sfinfo;
    sfinfo.channels = channels;
    sfinfo.samplerate = Fs;
    //sfinfo.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    file = sf_open("sound.wav", SFM_WRITE, &sfinfo);

    std::cout << "File Opened" << std::endl;

    return 0;
}

int SoundIO::play_sound(std::vector<float> data)
{
    PaError err;
    int floats_per_frame = FRAMES_PER_BUFFER * channels;
    for(unsigned i = 0; i + floats_per_frame < data.size(); i += floats_per_frame)
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
    std::cout << "Stream Stopped" << std::endl;

    Pa_Terminate();

    sf_write_sync(file);
    sf_close(file);
    std::cout << "File Closed" << std::endl;

    return 0;
}

std::vector<fpoint> get_samples_from_file(std::string filename)
{
    const int BUFFER_LEN = 1024;
    static float data[BUFFER_LEN];
    std::vector<fpoint> out;

    SNDFILE *infile;
    SF_INFO sfinfo;
    sfinfo.channels = 1;
    sfinfo.samplerate = Fs;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

    int readcount;

    filename.insert(0, "samples/");
    filename.append(".wav");

    if (! (infile = sf_open(filename.c_str(), SFM_READ, &sfinfo)))
    {
        std::cout << "no file named " << filename << std::endl;
        return out;
    }

    while ((readcount = sf_read_float(infile, data, BUFFER_LEN)))
    {
        for(int i = 0; i < readcount; ++i)
        {
            out.push_back(data[i]);
        }
    }

    sf_close (infile);

    return out;
}
