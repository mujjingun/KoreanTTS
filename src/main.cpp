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
    std::vector<float> out;
    SoundIO soundio;
    Synthesizer synth;

    bool use_sound = false;
    std::cout<<"Use sound I/O? ";
    fflush(stdin);
    if(getchar() == 'y')
    {
        use_sound = true;
        soundio.init_sound(1);
    }

    while(1)
    {
        out.clear();

        std::cout << "input: ";
        std::wstring in_uni = console_input();

        // decompose the syllables into hangul jamo
        std::vector<int> in = decompose(in_uni);

        if(in.empty()) break;

        for(int i : in)
        {
            std::cout << i << std::endl;
        }

        start = clock();

        synth.start(in);

        while(!synth.has_ended())
        {
            fpoint generated = synth.generate_sample();
            out.push_back((float)generated);
            //out.push_back((float)generated);
        }

        finish = clock();

        fpoint ft_rate = out.size() / ((finish - start) / (fpoint)CLOCKS_PER_SEC);
        std::cout << std::endl;
        std::cout << ft_rate << " samples per second." << std::endl;
        std::cout << ft_rate / Fs << " times faster than real-time" << std::endl;
        std::cout << "out buffer size: " << out.size() * 16 << " bytes\n" << std::endl;

        if(use_sound)
        {
            soundio.play_sound(out);
        }
    }

    if(use_sound)
    {
        soundio.end_sound();
    }

    return 0;
}
