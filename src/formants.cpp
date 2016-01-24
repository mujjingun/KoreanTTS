#include <cmath> // fmin, fmax
#include "formants.h"
#include "consonants.h"

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
    {360, 1021, 3068, 677, 2285, 3063},
    // 11. ㅚ (=ㅞ)
    {422, 1021, 3024, 650, 2377, 3068},
    // 12. ㅛ
    {344, 2814, 3471, 499, 1029, 3068},
    // 13. ㅜ
    {360, 1000, 3024, 360, 1021, 3024},
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

Consonants::Consonants()
: consonant_formants {
    // 0. ㄱ
    std::unique_ptr<Giyeok>(),
    // 1. ㄲ
    std::unique_ptr<Consonant>(),
    // 2. ㄴ
    std::unique_ptr<Consonant>(),
    // 3. ㄷ
    std::unique_ptr<Digeut>(),
    // 4. ㄸ
    std::unique_ptr<Consonant>(),
    // 5. ㄹ
    std::unique_ptr<Consonant>(),
    // 6. ㅁ
    std::unique_ptr<Consonant>(),
    // 7. ㅂ
    std::unique_ptr<Bieup>(),
    // 8. ㅃ
    std::unique_ptr<Ssangbieup>(),
    // 9. ㅅ
    std::unique_ptr<Siot>(),
    // 10. ㅆ
    std::unique_ptr<Consonant>(),
    // 11. ㅇ
    std::unique_ptr<Consonant>(),
    // 12. ㅈ
    std::unique_ptr<Consonant>(),
    // 13. ㅉ
    std::unique_ptr<Consonant>(),
    // 14. ㅊ
    std::unique_ptr<Consonant>(),
    // 15. ㅋ
    std::unique_ptr<Consonant>(),
    // 16. ㅌ
    std::unique_ptr<Consonant>(),
    // 17. ㅍ
    std::unique_ptr<Pieup>(),
    // 18. ㅎ
    std::unique_ptr<Consonant>(),
    // 19. null
    std::unique_ptr<Consonant>()
} {}

Consonant * Consonants::get(int i)
{
    return consonant_formants[i].get();
}

const Vowel & get_vowel_formants(int i)
{
    return vowel_formants[i];
}

fpoint clamp(fpoint s, fpoint e, fpoint x)
{
    return fmax(fmin(e, x), s);
}

fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex)
{
    return (e - s) * clamp(0, 1, (x - sx) / (ex - sx)) + s;
}
