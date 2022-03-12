#ifndef __SR_GAME_H__
#define __SR_GAME_H__

#include "Player.h"
#include <vector>
#include <deque>

namespace sr
{
    class Merchant;
    class Objective;
    
    class Game
    {
    public:
        static const unsigned int CLAIMABLE_COUNT = 5;
        static const unsigned int BUYABLE_COUNT = 6;
        
    public:
        Game();
        ~Game();

        void prepare();
        bool ended() const;
        void execute();

        unsigned int get_turn_id() const {
            return turn_id;
        }
        unsigned int get_round_id() const {
            return turn_id / players.size();
        }

        void add_player(Player& player) {
            players.emplace_back(player);
        }
        const std::vector<Player>& get_players() const {
            return players;
        }

        const Player& get_current_player() const {
            return players[turn_id % players.size()];
        }
        Player& get_current_player() {
            return players[turn_id % players.size()];
        }
        Player& next_player() {
            ++turn_id;
            return get_current_player();
        }

        unsigned int get_gold_coin_count() const {
            return gold_coin_count;
        }
        unsigned int get_silver_coin_count() const {
            return silver_coin_count;
        }
        const std::deque<const Objective*>& get_claimables() const {
            return claimables;
        }
        const Objective* get_claimable(unsigned int index) const {
            return claimables.at(index);
        }
        const std::deque<Buyable>& get_buyables() const {
            return buyables;
        }
        const Buyable* get_buyable(unsigned int index) const {
            return &(buyables.at(index));
        }

        unsigned int index_of(const Objective* claimable) const;

        void pop_claimable(unsigned int index);
        void pop_buyable(unsigned int index);
        void remove_gold_coin() {
            gold_coin_count--;
        }
        void remove_silver_coin()  {
            silver_coin_count--;
        }

        void add_deposit(const std::vector<Spice>& cost);

    protected:
        unsigned int get_objective_count_to_win() const;
    protected: 
    
        unsigned int turn_id;

        // the players of the game
        // the order of the vectore defines the turn order
        std::vector<Player> players;

        std::deque<const Objective*> objective_deck;
        std::deque<const Objective*> claimables; // always 5 cards
        unsigned int gold_coin_count; // associated with claimables.at(0) (aka claimables.front() )
        unsigned int silver_coin_count; // associated with claimables.at(1)

        std::deque<const Merchant*> merchant_deck;
        std::deque<Buyable> buyables; // always 6 cards
    };
}

#endif // __SR_GAME_H__
