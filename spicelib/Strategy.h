#ifndef __SR_STRATEGY_H__
#define __SR_STRATEGY_H__

#include "SpiceMap.h"

namespace sr {
class Action;
class Game;
class Player;

class Strategy {
   public:
    virtual ~Strategy() {}
    virtual Action* play_turn(const Game& game, const Player& player) = 0;
    virtual SpiceMap drop_spice(const Game& game, const Player& player) = 0;
};
}  // namespace sr

#endif  // __SR_STRATEGY_H__
