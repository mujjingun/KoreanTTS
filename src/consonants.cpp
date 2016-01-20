#include "consonants.h"

Consonant::Consonant(fpoint f1, fpoint f2, fpoint duration)
: f1(f1), f2(f2), duration(duration)
{
    // TODO
}

////////////////////////////////////////
//
//  ㅂ : Bieup
//
////////////////////////////////////////

Bieup::Bieup()
: Consonant(20, 800, 0.02 + 0.035)
{

}
