#ifndef __SR_PLAYER_H__
#define __SR_PLAYER_H__

#include <memory>
#include <unordered_set>
#include <vector>

#include "Merchant.h"
#include "Objective.h"
#include "SpiceMap.h"

namespace sr {
class Game;
class Action;
class Strategy;

class Player {
   public:
    static const int MAX_SPICE_IN_CARAVAN = 10;
    static const unsigned int GOLD_COIN_SCORE = 3;
    static const unsigned int SILVER_COIN_SCORE = 1;

   public:
    Player(const std::string& name, Strategy* strategy);
    ~Player();

    const std::string& get_name() const { return name; }

    const SpiceMap& get_caravan() const { return caravan; }
    SpiceMap& get_caravan() { return caravan; }
    const std::unordered_set<const Merchant*>& get_hand() const { return hand; }
    const std::unordered_set<const Merchant*>& get_discard() const {
        return discard_pile;
    }
    const std::unordered_set<const Objective*>& get_objectives() const {
        return objectives;
    }
    const Strategy* get_strategy() const { return strategy.get(); }
    unsigned int get_score(bool with_spice_bonuses = true) const;

    Action* play_turn(const Game& game) const;
    void drop_spice_if_needed(const Game& game);

    void play(const Production& card);
    void play(const Trade& card, unsigned int times);
    void play(const Upgrade& card, const SpiceMap& upgrade_map);
    void buy(const Buyable& card, const std::vector<Spice>& cost);
    void sleep();
    void claim(const Objective& card);

    void add_gold_coin();
    void add_silver_coin();

    const Upgrade* get_starter_upgrade() const { return &starter_upgrade; }
    const Production* get_starter_production() const {
        return &starter_production;
    }

   private:
    std::unordered_set<const Merchant*>::iterator check_merchant_card_in_hand(
        const Merchant& card);
    void discard(std::unordered_set<const Merchant*>::iterator hand_iterator);

   protected:
    std::string name;
    std::shared_ptr<Strategy> strategy;

    SpiceMap caravan;
    // hand
    std::unordered_set<const Merchant*> hand;
    // discard_pile
    std::unordered_set<const Merchant*> discard_pile;
    // objectives
    std::unordered_set<const Objective*> objectives;
    // gold coins
    unsigned int gold_coins;
    // silver coins
    unsigned int silver_coins;

   private:
    Upgrade starter_upgrade;
    Production starter_production;
};

}  // namespace sr

#endif  //__SR_PLAYER_H__
