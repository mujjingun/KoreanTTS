#include "encoding.h"

#define READFROMCONSOLE

#ifdef READFROMCONSOLE

#include <iostream>
#include <windows.h>

std::wstring console_input()
{
    std::string input;
    std::cin >> input;
    wchar_t tmp[1000] = {0};
    int len = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, input.c_str(), input.length(), tmp, 1000);
    tmp[len] = 0;
    std::wstring str(tmp);
    return str;
}

#else

#include <codecvt>
int console_input ( std::wstring & str )
{
    std::ifstream file("Read.txt");
    std::string str;
    std::wstring_convert<std::codecvt_utf8<char16_t>> convert;
}

#endif

#define FIRST_HANGUL_UNICODE 0xAC00
#define LAST_HANGUL_UNICODE 0xD7A3
#define NUM_CHOSUNGS 19
#define NUM_JUNGSUNGS 21
#define NUM_JONGSUNGS 28

std::vector<int> decompose( std::wstring const & str )
{
    std::vector<int> out;
    for(size_t i = 0; i < str.length(); i++)
    {
        int code = str[i] - FIRST_HANGUL_UNICODE;

        if(code < 0 || code > LAST_HANGUL_UNICODE - FIRST_HANGUL_UNICODE) continue;

        int jong = code % NUM_JONGSUNGS;
        code /= NUM_JONGSUNGS;
        int jung = code % NUM_JUNGSUNGS;
        code /= NUM_JUNGSUNGS;
        int cho = code;

        if(cho != 11) // 'ㅇ'
            out.push_back(cho + 100);
        out.push_back(jung);
        if(jong != 0)
            out.push_back(jong + 200);
    }
    return out;
}
