#ifndef __SR_SPICE_H__
#define __SR_SPICE_H__

#include <cstddef>

namespace sr
{
    enum class Spice
    {
        yellow = 0,
        red = 1,
        green = 2,
        brown = 3,
    };
    static const size_t SPICE_TYPE_COUNT = 4;

    inline int value_of(Spice spice)
    {
        return static_cast<int>(spice) + 1;
    }

    inline unsigned int is_upgradable(Spice spice) {
        return static_cast<int>(Spice::brown) - static_cast<int>(spice);
    }
}

#endif // __SR_SPICE_H__
