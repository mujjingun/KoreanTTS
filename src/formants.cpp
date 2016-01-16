#include <cmath>
#include "formants.h"

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

Vowel* formants[] = {
    // 0. ㅏ
    new Monophthong(986, 1794, 2957),
    // 1. ㅐ
    new Monophthong(677, 2285, 3063),
    // 2. ㅑ
    new Diphthong(20, 0),
    // 3. ㅒ
    new Diphthong(20, 1),
    // 4. ㅓ
    new Monophthong(765, 1371, 3009),
    // 5. ㅔ
    new Monophthong(650, 2377, 3068),
    // 6. ㅕ
    new Diphthong(20, 4),
    // 7. ㅖ
    new Diphthong(20, 5),
    // 8. ㅗ
    new Monophthong(499, 1029, 3068),
    // 9. ㅘ
    new Diphthong(8, 0),
    // 10. ㅙ
    new Diphthong(8, 1),
    // 11. ㅚ
    new Diphthong(13, 5),
    // 12. ㅛ
    new Diphthong(20, 8),
    // 13. ㅜ
    new Monophthong(422, 1021, 3024),
    // 14. ㅝ
    new Diphthong(13, 4),
    // 15. ㅞ
    new Diphthong(13, 5),
    // 16. ㅟ
    new Diphthong(13, 20),
    // 17. ㅠ
    new Diphthong(20, 13),
    // 18. ㅡ
    new Monophthong(447, 1703, 2997),
    // 19. ㅢ
    new Diphthong(18, 20),
    // 20. ㅣ
    new Monophthong(344, 2814, 3471)
};

Vowel* get_formant(int i)
{
    return formants[i];
}

fpoint clamp(fpoint s, fpoint e, fpoint x)
{
    return fmax(fmin(e, x), s);
}

fpoint linear(fpoint s, fpoint e, fpoint x, fpoint sx, fpoint ex)
{
    return (e - s) * clamp(0, 1, (x - sx) / (ex - sx)) + s;
}
