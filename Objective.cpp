#include "Objective.h"

#include "Player.h"

using namespace sr;

unsigned int Objective::distance(const SpiceMap& that_spice_map) const {
    return sr::distance(spice_map.negate(), that_spice_map);
}

unsigned int Objective::distance(const Player& player) const {
    return distance(player.get_caravan());
}
