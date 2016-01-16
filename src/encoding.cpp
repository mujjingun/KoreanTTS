#include <windows.h>
#include <cstdio>
#include "encoding.h"

int console_input ( wchar_t * str, const int size )
{
    fflush(stdin);
    char input[size * 3];
    fgets(input, size * 3, stdin);
    return MultiByteToWideChar(CP_ACP, MB_COMPOSITE, input, strlen(input), str, size);
}

#define FIRST_HANGUL_UNICODE 0xAC00
#define LAST_HANGUL_UNICODE 0xD7A3
#define NUM_CHOSUNGS 19
#define NUM_JUNGSUNGS 21
#define NUM_JONGSUNGS 28

std::vector<int> decompose( wchar_t * str, int len )
{
    std::vector<int> out;
    int c = 0;
    for(int i = 0; i < len; i++)
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
