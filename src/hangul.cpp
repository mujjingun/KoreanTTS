#include "hangul.h"

std::vector<int> to_phonetic(std::vector<int> str)
{
    // TODO
    return str;
}

// choseong : 1xx
// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
// ㄱ ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ
//
// jungseong : 2xx
// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20
// ㅏ ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ
//
// jongseong : 3xx
// 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
// ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅄ ㅅ ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ
//
// to
//
// 0       1   2  3  4   5  6     7  8   9  10 11 12     13 14 15 16 17 18 19
// (space) ㄱi ㄲ ㄴ ㄷi ㄸ ㄹi/l ㅁ ㅂi ㅃ ㅅ ㅆ ㅇ(ng) ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ
//
// 20       21  22  23  24  25  26
// ㅇ(null) ㄱm ㄷm ㅂm ㅅm ㅈm ㄹm
//
// 50 51 52 53 54 55 56 57 58 59 60    61    62 63 64 65 66// ㅏ ㅑ ㅓ ㅕ ㅗ ㅛ ㅜ ㅠ ㅡ ㅣ ㅐ/ㅔ ㅙ/ㅞ ㅚ ㅟ ㅘ ㅝ ㅖ/ㅒ
//
std::vector<int> to_narrow(std::vector<int> str)
{
    std::vector<int> out;

    out.push_back(0);

    for(unsigned i = 1; i < str.size() - 1; i++)
    {
        int prev = str[i - 1];
        int ch = str[i];

        switch(ch)
        {
        // space
        case 0:
        default:
            out.push_back(0);
            break;

        // cho 'ㄱ'
        case 100:
            if(prev == 0)
            {
                out.push_back(1);
            }
            else
            {
                out.push_back(21);
            }
            break;

        case 102:
            out.push_back(3);
            break;

        case 103:
            if(prev == 0)
            {
                out.push_back(4);
            }
            else
            {
                out.push_back(22);
            }
            break;

        case 105:
            out.push_back(6);
            break;

        // cho 'ㅇ'
        case 111:
            if(prev == 0)
            {
                out.push_back(20);
            }
            break;

        // jung 'ㅏ'
        case 200:
            out.push_back(50);
            break;
        // jung 'ㅐ'
        case 201:
            out.push_back(60);
            break;
        // jung 'ㅑ'
        case 202:
            out.push_back(51);
            break;
        // jung 'ㅒ'
        case 203:
            out.push_back(66);
            break;
        // TODO
        }
    }

    out.push_back(0);

    return out;
}
