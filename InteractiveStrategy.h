#ifndef __SR_INTERACTIVESTRATEGY_H__
#define __SR_INTERACTIVESTRATEGY_H__

#include "Strategy.h"

namespace sr {
class InteractiveStrategy : public Strategy {
   public:
    ~InteractiveStrategy() override {}
    Action* play_turn(const Game& game, const Player& player) override;
    SpiceMap drop_spice(const Game& game, const Player& player) override;

   protected:
    Action* play(const Game& game, const Player& player);
    Action* buy(const Game& game, const Player& player);
    Action* sleep(const Game& game, const Player& player);
    Action* claim(const Game& game, const Player& player);
};
}  // namespace sr

#endif  // __SR_INTERACTIVESTRATEGY_H__
