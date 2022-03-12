#ifndef __SR_OBJECTIVE_H__
#define __SR_OBJECTIVE_H__

#include "SpiceMap.h"
#include <iomanip>

namespace sr
{
    class Player;

    class Objective
    {
    public:
        Objective(unsigned int value, std::initializer_list<int> spice_map) : 
        value(value),
        spice_map(spice_map) {
        }
        unsigned int get_value() const {
            return value;
        }
        const SpiceMap& get_spice_map() const {
            return spice_map;
        }

        unsigned int distance(const SpiceMap& spice_map) const;
        unsigned int distance(const Player& player) const;

        friend std::ostream& operator<<(std::ostream& stream, const Objective& objective)
        {
            return stream << "{" << 
                std::setw(2) <<
                objective.value << ", " <<
                objective.spice_map <<
                "}";
        }

    protected:
        unsigned int value;
        SpiceMap spice_map;
    };
}

#endif // __SR_OBJECTIVE_H__
