#ifndef __SR_GAME_H__
#define __SR_GAME_H__

#include <deque>
#include <vector>

#include "Player.h"

namespace sr {
class Merchant;
class Objective;

class Game {
   public:
    static const unsigned int CLAIMABLE_COUNT = 5;
    static const unsigned int BUYABLE_COUNT = 6;

   public:
    Game();
    ~Game();

    void prepare(unsigned int seed);
    /**
     * @brief Check if the end game condition is reached
     *
     * @returns true if the current player reached a fix number of objectives
     * @see get_objective_count_to_win
     */
    bool ended() const;
    /**
     * @brief The main game loop.
     *
     * For each player and until the end game is reached :
     *  - ask for an action to play
     *  - ensure the validity of the action
     *  - execute the action
     *  - ask to drop the exeeding spice from the caravan
     */
    void execute();

    unsigned int get_turn_id() const { return turn_id; }
    unsigned int get_round_id() const { return turn_id / players.size(); }

    void add_player(Player& player) { players.emplace_back(player); }
    const std::vector<Player>& get_players() const { return players; }

    const Player& get_current_player() const {
        return players[turn_id % players.size()];
    }
    Player& get_current_player() { return players[turn_id % players.size()]; }
    Player& next_player() {
        ++turn_id;
        return get_current_player();
    }

    unsigned int get_gold_coin_count() const { return gold_coin_count; }
    unsigned int get_silver_coin_count() const { return silver_coin_count; }
    const std::deque<const Objective*>& get_claimables() const {
        return claimables;
    }
    const Objective* get_claimable(unsigned int index) const {
        return claimables.at(index);
    }
    const std::deque<Buyable>& get_buyables() const { return buyables; }
    const Buyable* get_buyable(unsigned int index) const {
        return &(buyables.at(index));
    }

    unsigned int index_of(const Objective* claimable) const;

    void pop_claimable(unsigned int index);
    void pop_buyable(unsigned int index);
    void remove_gold_coin() { gold_coin_count--; }
    void remove_silver_coin() { silver_coin_count--; }

    void add_deposit(const std::vector<Spice>& cost);

    /**
     * @brief Returns the number of objective to reach to win the game
     *
     * <table>
     *  <tr><th>Player count<th>Objectives to win
     *  <tr><td>1<td>6
     *  <tr><td>2<td>6
     *  <tr><td>3<td>6
     *  <tr><td>4<td>5
     *  <tr><td>5<td>5
     * </table>
     */
    unsigned int get_objective_count_to_win() const;

   protected:
    unsigned int turn_id;

    // the players of the game
    // the order of the vectore defines the turn order
    std::vector<Player> players;

    std::deque<const Objective*> objective_deck;
    std::deque<const Objective*> claimables;  // always 5 cards
    unsigned int gold_coin_count;    // associated with claimables.at(0) (aka
                                     // claimables.front() )
    unsigned int silver_coin_count;  // associated with claimables.at(1)

    std::deque<const Merchant*> merchant_deck;
    std::deque<Buyable> buyables;  // always 6 cards
};
}  // namespace sr

#endif  // __SR_GAME_H__
