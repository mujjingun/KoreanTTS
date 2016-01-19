#include <iostream>
#include <ctime>
#include <string>
#include <vector>

#include "main.h"
#include "soundio.h"
#include "synthesizer.h"
#include "encoding.h"

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
        soundio->init_sound(1);
    }

    while(1)
    {
        out.clear();

        std::cout << "input: ";
        std::wstring in_uni;
        in_uni = console_input();

        std::vector<int> in;
        in = decompose(in_uni);

        if(in.empty()) break;

        for(size_t i = 0; i < in.size(); i++)
        {
            std::cout << in[i] << std::endl;
        }

        start = clock();

        synth.start(in);

        while(!synth.has_ended())
        {
            int16_t generated = synth.generate_sample();
            out.push_back(generated);
            //out.push_back(-generated);
        }

        finish = clock();

        fpoint ft_rate = out.size() / ((finish - start) / (fpoint)CLOCKS_PER_SEC);
        std::cout << std::endl;
        std::cout << ft_rate << " samples per second." << std::endl;
        std::cout << "lags " << Fs - ft_rate << "s per second" << std::endl;
        std::cout << "out buffer size: " << out.size() * 16 << " bytes\n" << std::endl;

        if(use_sound)
        {
            soundio->play_sound(&out[0], out.size());
        }
    }

    if(use_sound) soundio->end_sound();

    return 0;
}
