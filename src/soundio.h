#ifndef SOUNDIO_H_INCLUDED
#define SOUNDIO_H_INCLUDED

#include <portaudio.h>
#include <sndfile.h>
#include "main.h"

class SoundIO
{
    private:

        const int FRAMES_PER_BUFFER = 1024;

        PaStream *stream;

        SNDFILE *file;

    public:

        SoundIO(){};
        virtual ~SoundIO(){};

        int init_sound();
        int play_sound(int16_t* data, int N);
        int end_sound();
        static int16_t normalize(const fpoint& x);

};


#endif // SOUNDIO_H_INCLUDED
