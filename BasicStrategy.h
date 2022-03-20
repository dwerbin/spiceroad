#ifndef __SR_BASICSTRATEGY_H__
#define __SR_BASICSTRATEGY_H__

#include <set>

#include "Strategy.h"

namespace sr {
class Objective;
class Merchant;
class Trade;

// mean :
//        51.24 turns
//        68.53 points
class BasicStrategy : public Strategy {
   public:
    BasicStrategy();
    ~BasicStrategy() override {}
    Action* play_turn(const Game& game, const Player& player) override;
    SpiceMap drop_spice(const Game& game, const Player& player) override;

    const Objective* get_focus() const { return focus; }

   protected:
    static const Objective* select_objective(const Game& game,
                                             const Player& player);
    SpiceMap drop_spice(const SpiceMap& caravan, unsigned int max_spice) const;
    void build_all_possible_drop_map(std::set<SpiceMap>& result,
                                     const SpiceMap& caravan0,
                                     const SpiceMap& caravan,
                                     unsigned int quantity_to_drop) const;
    static unsigned int compute_max_times_to_play(const Trade* trade);
    static std::vector<SpiceMap> compute_all_upgrade_combination(
        unsigned int max_upgrade_times, const SpiceMap& caravan);
    Action* try_to_apply_all_combination(const Merchant* merchant,
                                         const SpiceMap& caravan,
                                         unsigned int& distance,
                                         unsigned int* action_value) const;

   protected:
    const Objective* focus;
};
}  // namespace sr

#endif  // __SR_BASICSTRATEGY_H__
