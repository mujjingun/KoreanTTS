#include <iostream>
#include <ctime>
#include <vector>

#include "main.h"
#include "soundio.h"
#include "synthesizer.h"
#include "encoding.h"

#define INPUT_SIZE 1000

int main(int argc, char *argv[])
{
    clock_t start, finish;
    std::vector<int16_t> out;
    SoundIO *soundio;
    Synthesizer synth;

    bool use_sound = false;
    std::cout<<"Use sound I/O? ";
    fflush(stdin);
    if(getchar() == 'y') {
        use_sound = true;
        soundio = new SoundIO();
    }

    if(use_sound) soundio->init_sound();

    while(1)
    {
        out.clear();

        std::cout << "input: ";
        wchar_t in_uni[INPUT_SIZE];
        int len_uni = console_input(in_uni, INPUT_SIZE);

        std::vector<int> in;
        in = decompose(in_uni, len_uni);

        if(in.empty()) break;

        for(size_t i = 0; i < in.size(); i++)
        {
            std::cout << in[i] << std::endl;
        }

        start = clock();

        synth.start(in);

        while(!synth.has_ended())
            out.push_back(synth.generate_sample());

        finish = clock();

        fpoint ft_rate = out.size() / ((finish - start) / (fpoint)CLOCKS_PER_SEC);
        std::cout << std::endl;
        std::cout << ft_rate << " samples per second." << std::endl;
        std::cout << "lags " << Fs - ft_rate << "s per second" << std::endl;
        std::cout << "out buffer size: " << out.size() * 16 << " bytes" << std::endl;

        if(use_sound)
        {
            std::cout << "press enter to play" << std::endl;
            fflush(stdin);
            if(getchar() == 'q') break;

            soundio->play_sound(&out[0], out.size());
        }
    }

    if(use_sound) soundio->end_sound();

    return 0;
}
