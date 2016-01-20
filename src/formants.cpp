#include <cmath> // fmin, fmax
#include "formants.h"
#include "consonants.h"

/*
Vowel* formantsMale[] = {
    // 0. ㅏ
    new Monophthong(738, 1372, 2573),
    // 1. ㅐ
    new Monophthong(591, 1849, 2597),
    // 2. ㅑ
    new Diphthong(20, 0),
    // 3. ㅒ
    new Diphthong(20, 1),
    // 4. ㅓ
    new Monophthong(608, 1121, 2683),
    // 5. ㅔ
    new Monophthong(490, 1968, 2644),
    // 6. ㅕ
    new Diphthong(20, 4),
    // 7. ㅖ
    new Diphthong(20, 5),
    // 8. ㅗ
    new Monophthong(453, 945, 2674),
    // 9. ㅘ
    new Diphthong(8, 0),
    // 10. ㅙ
    new Diphthong(8, 1),
    // 11. ㅚ
    new Diphthong(13, 5),
    // 12. ㅛ
    new Diphthong(20, 8),
    // 13. ㅜ
    new Monophthong(369, 981, 2565),
    // 14. ㅝ
    new Diphthong(13, 4),
    // 15. ㅞ
    new Diphthong(13, 5),
    // 16. ㅟ
    new Diphthong(13, 20),
    // 17. ㅠ
    new Diphthong(20, 13),
    // 18. ㅡ
    new Monophthong(405, 1488, 2497),
    // 19. ㅢ
    new Diphthong(18, 20),
    // 20. ㅣ
    new Monophthong(342, 2219, 3047)
};
*/

const Vowel vowel_formants[] = {
    // 0. ㅏ
    {986, 1794, 2957, 986, 1794, 2957},
    // 1. ㅐ
    {677, 2285, 3063, 677, 2285, 3063},
    // 2. ㅑ
    {344, 2814, 3471, 986, 1794, 2957},
    // 3. ㅒ
    {344, 2814, 3471, 677, 2285, 3063},
    // 4. ㅓ
    {765, 1371, 3009, 765, 1371, 3009},
    // 5. ㅔ
    {650, 2377, 3068, 650, 2377, 3068},
    // 6. ㅕ
    {344, 2814, 3471, 765, 1371, 3009},
    // 7. ㅖ
    {344, 2814, 3471, 650, 2377, 3068},
    // 8. ㅗ
    {499, 1029, 3068, 499, 1029, 3068},
    // 9. ㅘ
    {499, 1029, 3068, 986, 1794, 2957},
    // 10. ㅙ
    {499, 1029, 3068, 677, 2285, 3063},
    // 11. ㅚ (=ㅞ)
    {422, 1021, 3024, 650, 2377, 3068},
    // 12. ㅛ
    {344, 2814, 3471, 499, 1029, 3068},
    // 13. ㅜ
    {360, 1000, 3024, 380, 1021, 3024},
    // 14. ㅝ
    {422, 1021, 3024, 765, 1371, 3009},
    // 15. ㅞ
    {422, 1021, 3024, 650, 2377, 3068},
    // 16. ㅟ
    {333, 1192, 3024, 344, 2814, 3471},
    // 17. ㅠ
    {344, 2814, 3471, 422, 1021, 3024},
    // 18. ㅡ
    {447, 1703, 2997, 447, 1703, 2997},
    // 19. ㅢ
    {447, 1703, 2997, 344, 2814, 3471},
    // 20. ㅣ
    {344, 2814, 3471, 344, 2814, 3471}
};

Consonant *consonant_formants[] = {
    // 0. ㄱ
    new Consonant(),
    // 1. ㄲ
    new Consonant(),
    // 2. ㄴ
    new Consonant(),
    // 3. ㄷ
    new Consonant(),
    // 4. ㄸ
    new Consonant(),
    // 5. ㄹ
    new Consonant(),
    // 6. ㅁ
    new Consonant(),
    // 7. ㅂ
    new Bieup(),
    // 8. ㅃ
    new Consonant(),
    // 9. ㅅ
    new Consonant(),
    // 10. ㅆ
    new Consonant(),
    // 11. ㅇ
    new Consonant(),
    // 12. ㅈ
    new Consonant(),
    // 13. ㅉ
    new Consonant(),
    // 14. ㅊ
    new Consonant(),
    // 15. ㅋ
    new Consonant(),
    // 16. ㅌ
    new Consonant(),
    // 17. ㅍ
    new Consonant(),
    // 18. ㅎ
    new Consonant()
};
// ieung
Consonant * null_consonant = new Consonant();

const Vowel & get_vowel_formants(int i)
{
    return vowel_formants[i];
}

Consonant * get_consonant_formants(int i)
{
    return consonant_formants[i];
}

fpoint clamp(fpoint s, fpoint e, fpoint x)
{
    return fmax(fmin(e, x), s);
}

fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex)
{
    return (e - s) * clamp(0, 1, (x - sx) / (ex - sx)) + s;
}
