#ifndef __SR_STRATEGY_H__
#define __SR_STRATEGY_H__

#include <set>
#include "SpiceMap.h"

namespace sr
{
    class Action;
    class Game;
    class Player;
    class Objective;
    class Merchant;
    class Trade;

    class Strategy
    {
    public:
        virtual ~Strategy() {}
        virtual Action* play_turn(const Game& game, const Player& player) = 0;
        virtual SpiceMap drop_spice(const Game& game, const Player& player) = 0;
    };

    class BasicStrategy : public Strategy
    {
    public:
        BasicStrategy();
        ~BasicStrategy() override {}
        Action* play_turn(const Game& game, const Player& player) override;
        SpiceMap drop_spice(const Game& game, const Player& player) override;

        const Objective* get_focus() const {
            return focus;
        }

    protected:
        static const Objective* select_objective(const Game& game, const Player& player);
        SpiceMap drop_spice(const SpiceMap& caravan, unsigned int max_spice) const;
        void build_all_possible_drop_map(std::set<SpiceMap>& result, const SpiceMap& caravan0, const SpiceMap& caravan, unsigned int quantity_to_drop) const;
        static unsigned int compute_max_times_to_play(const Trade* trade);
        static std::vector<SpiceMap> compute_all_upgrade_combination(unsigned int max_upgrade_times, const SpiceMap& caravan);
        Action* try_to_apply_all_combination(const Merchant* merchant, const SpiceMap& caravan, unsigned int& distance, unsigned int* action_value) const;
    protected:
        const Objective* focus;
    };

    class InteractiveStrategy : public Strategy
    {
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
}

#endif // __SR_STRATEGY_H__
