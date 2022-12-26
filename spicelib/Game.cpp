#include "Game.h"

#include <algorithm>
#include <chrono>
#include <iterator>
#include <memory>
#include <random>

#include "Action.h"
#include "AsciiPrinter.h"
#include "BasicStrategy.h"
#include "SpiceMap.h"
#include "ansi.h"

using namespace sr;

static const SpiceMap initial_caravans[] = {
    {3, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {3, 1, 0, 0}, {3, 1, 0, 0},
};

static const Objective objectives[] = {
    {6, {-2, -2, -0, -0}},  {7, {-3, -2, -0, -0}},  {8, {-2, -3, -0, -0}},
    {8, {-2, -0, -2, -0}},  {8, {-0, -4, -0, -0}},  {9, {-2, -1, -0, -1}},
    {9, {-3, -0, -2, -0}},  {10, {-2, -0, -0, -2}}, {10, {-0, -2, -2, -0}},
    {10, {-0, -5, -0, -0}}, {11, {-2, -0, -3, -0}}, {11, {-3, -0, -0, -2}},
    {12, {-1, -1, -1, -1}}, {12, {-1, -0, -2, -1}}, {12, {-0, -2, -0, -2}},
    {12, {-0, -3, -2, -0}}, {12, {-0, -2, -1, -1}}, {12, {-0, -0, -4, -0}},
    {13, {-2, -2, -2, -0}}, {13, {-0, -2, -3, -0}}, {14, {-3, -1, -1, -1}},
    {14, {-2, -0, -0, -3}}, {14, {-0, -3, -0, -2}}, {14, {-0, -0, -2, -2}},
    {15, {-2, -2, -0, -2}}, {15, {-0, -0, -5, -0}}, {16, {-1, -3, -1, -1}},
    {16, {-0, -2, -0, -3}}, {16, {-0, -0, -0, -4}}, {17, {-2, -0, -2, -2}},
    {17, {-0, -0, -3, -2}}, {18, {-1, -1, -3, -1}}, {18, {-0, -0, -2, -3}},
    {19, {-0, -2, -2, -2}}, {20, {-1, -1, -1, -3}}, {20, {-0, -0, -0, -5}},
};

static const Production productions[] = {
    {3, 0, 0, 0}, {4, 0, 0, 0}, {1, 1, 0, 0}, {2, 1, 0, 0},
    {1, 0, 1, 0}, {0, 2, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1},
};

static const Trade trades[] = {
    {-2, 2, 0, 0}, {-2, 0, 1, 0}, {-3, 3, 0, 0},  {-3, 1, 1, 0},  {-3, 0, 0, 1},
    {-4, 0, 2, 0}, {-4, 0, 1, 1}, {-5, 0, 3, 0},  {-5, 0, 0, 2},  {3, -1, 0, 0},
    {3, -2, 1, 0}, {0, -2, 2, 0}, {2, -2, 0, 1},  {2, -3, 2, 0},  {0, -3, 3, 0},
    {1, -3, 1, 1}, {0, -3, 0, 2}, {4, 1, -1, 0},  {1, 2, -1, 0},  {0, 2, -1, 0},
    {2, 3, -2, 0}, {2, 1, -2, 1}, {0, 2, -2, 1},  {0, 0, -2, 2},  {0, 0, -3, 3},
    {2, 2, 0, -1}, {0, 3, 0, -1}, {3, 0, 1, -1},  {1, 1, 1, -1},  {0, 0, 2, -1},
    {1, 1, 3, -2}, {0, 3, 2, -2}, {-1, -1, 0, 1}, {-2, 0, -1, 2},
};

static const Upgrade upgrades[] = {
    {3},
};

Game::Game()
    : turn_id(-1),
      players(),
      objective_deck(),
      claimables(),
      gold_coin_count(0),
      silver_coin_count(0),
      merchant_deck(),
      buyables() {
    // create the objective deck
    objective_deck.resize(std::size(objectives));
    for (size_t i = 0; i < std::size(objectives); ++i) {
        objective_deck.at(i) = (objectives + i);
    }

    // create the merchant deck
    merchant_deck.resize(std::size(productions) + std::size(trades) +
                         std::size(upgrades));
    size_t merchant_index = 0;
    for (size_t i = 0; i < std::size(productions); ++i) {
        merchant_deck.at(merchant_index++) = (productions + i);
    }
    for (size_t i = 0; i < std::size(trades); ++i) {
        merchant_deck.at(merchant_index++) = (trades + i);
    }
    for (size_t i = 0; i < std::size(upgrades); ++i) {
        merchant_deck.at(merchant_index++) = (upgrades + i);
    }
}

Game::~Game() {}

void Game::prepare(unsigned int seed) {
    // shuffle the cards
    std::default_random_engine generator(seed);
    std::cout << "RAND SEED : " << seed << std::endl;
    std::shuffle(objective_deck.begin(), objective_deck.end(), generator);
    std::shuffle(merchant_deck.begin(), merchant_deck.end(), generator);

    // if (players.size() < 2)
    // {
    //     throw "Cannot prepare the game, min : 2 players";
    // }
    if (players.size() > 5) {
        throw "Cannot prepare the game, max : 5 players";
    }

    // prepare the board
    gold_coin_count = silver_coin_count = 2 * players.size();

    // takes 5 cards from the objective deck and make them claimable
    for (unsigned int i = 0; i < CLAIMABLE_COUNT; ++i) {
        claimables.push_back(objective_deck.front());
        objective_deck.pop_front();
    }

    // takes 6 cards from the merchant deck and make them buyable
    for (unsigned int i = 0; i < BUYABLE_COUNT; ++i) {
        buyables.emplace_back(merchant_deck.front());
        merchant_deck.pop_front();
    }

    for (unsigned int i = 0; i < players.size(); ++i) {
        Player& player = players[i];
        // distribute the initials spices
        player.get_caravan() = initial_caravans[i];
    }
}

bool Game::ended() const {
    return (get_current_player().get_objectives().size() >=
            get_objective_count_to_win());
}

void Game::execute() {
    AsciiPrinter printer;
    do {
        Player& player = next_player();
        if (get_round_id() > 150) {
            printer.print(std::cout, *this);
            std::cout << player.get_name() << "'s turn:" << std::setw(41)
                      << std::right << "R" << get_round_id() << "T"
                      << get_turn_id() << std::endl;
            printer.print(std::cout, player);
            exit(0);
        }
        //-------------------
        printer.print(std::cout, *this);
        std::cout << player.get_name() << "'s turn:" << std::setw(41)
                  << std::right << "R" << get_round_id() << "T" << get_turn_id()
                  << std::endl;
        printer.print(std::cout, player);
        //-------------------

        std::shared_ptr<Action> action(player.play_turn(*this));

#if 0
        std::cout << "T" << std::setfill('0') << std::setw(3) << get_turn_id()
                  << "R" << std::setw(2) << get_round_id() << std::setfill(' ')
                  << std::endl;
        std::cout << "          C:" << player.get_caravan() << std::endl;
        if (const BasicStrategy* basic =
                dynamic_cast<const BasicStrategy*>(player.get_strategy())) {
            std::cout << "       -> F:" << *basic->get_focus() << std::endl;
        }
        std::cout << "          " << (*action);
        if (ClaimAction* claim = dynamic_cast<ClaimAction*>(action.get())) {
            const Objective* claimable =
                get_claimable(claim->get_claimable_index());
            std::cout << "                                         "
                      << *claimable;
        } else if (BuyAction* buy = dynamic_cast<BuyAction*>(action.get())) {
            const Buyable* buyable = get_buyable(buy->get_cost().size());
            std::cout << "                                         "
                      << *buyable;
        }
        std::cout << std::endl;
#endif

        if (action->valid(*this, player)) {
            action->execute(*this, player);
        } else {
            throw std::runtime_error(
                "The game cannot execute an invalid action !");
        }
        player.drop_spice_if_needed(*this);
    } while (!ended());
}

unsigned int Game::get_objective_count_to_win() const {
    if (players.size() <= 3) {
        return 6;
    } else {
        return 5;
    }
}

void Game::pop_claimable(unsigned int index) {
    claimables.erase(claimables.begin() + index);
    claimables.push_back(objective_deck.front());
    objective_deck.pop_front();
}

void Game::pop_buyable(unsigned int index) {
    buyables.erase(buyables.begin() + index);
    buyables.emplace_back(merchant_deck.front());
    merchant_deck.pop_front();
}

void Game::add_deposit(const std::vector<Spice>& cost) {
    for (unsigned int i = 0; i < cost.size(); ++i) {
        buyables.at(i).add_deposit(cost[i]);
    }
}

unsigned int Game::index_of(const Objective* claimable) const {
    auto ite = find(claimables.begin(), claimables.end(), claimable);
    if (ite != claimables.end()) {
        return (ite - claimables.begin());
    }
    return UINT_MAX;
}