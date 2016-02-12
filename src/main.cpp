#include <iostream>
#include <ctime>
#include <string>

#include "main.h"
#include "soundio.h"
#include "synthesizer.h"
#include "encoding.h"
#include "hangul.h"

int main()
{
    //test();

    std::vector<float> out;
    SoundIO soundio;
    Synthesizer synth;

    std::cout.sync_with_stdio(false);

    soundio.init_sound(1);

    while(1)
    {
        out.clear();

        // get console input
        std::cout << "input: ";
        std::wstring in_uni = console_input();

        // decompose the syllables into hangul jamo
        std::vector<int> in = decompose(in_uni);

        std::cout << "\ndecomposition" << std::endl;
        for(int i : in)
        {
            std::cout << i << ", ";
        }

        // to phonetic hangul
        in = to_phonetic(in);

        std::cout << "\nphonetic" << std::endl;
        for(int i : in)
        {
            std::cout << i << ", ";
        }

        // to narrow transcription
        in = to_narrow(in);

        std::cout << "\nnarrow" << std::endl;
        for(int i : in)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;

        if(in.size() <= 2) break;

        clock_t start = clock();

        synth.start(in);

        while(!synth.has_ended())
        {
            auto generated = synth.generate_frame();
            out.insert(out.end(), generated.begin(), generated.end());
        }

        clock_t finish = clock();

        fpoint ft_rate = out.size() / ((finish - start) / (fpoint)CLOCKS_PER_SEC);
        std::cout << std::endl;
        std::cout << ft_rate << " samples per second." << std::endl;
        std::cout << ft_rate / Fs << " times faster than real-time" << std::endl;
        std::cout << "out buffer size: " << out.size() * 16 << " bytes\n" << std::endl;

        std::cout << "playing sound.." << std::endl;
        soundio.play_sound(out);
    }

    soundio.end_sound();

    return 0;
}
