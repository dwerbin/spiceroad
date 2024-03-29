#include "InteractiveStrategy.h"

#include <iostream>
#include <unordered_set>

#include "Action.h"
#include "Game.h"
#include "Merchant.h"
#include "Player.h"

using namespace sr;

Action* InteractiveStrategy::play_turn(const Game& game, const Player& player) {
    Action* action = nullptr;
    do {
        if (action != nullptr) {
            delete action;
            std::cout << "INVALID ACTION ! TRY ANOTHER ACTION !" << std::endl;
        }
        action = play_turn_internal(game, player);
    } while (action == nullptr || !(action->valid(game, player)));
    return action;
}

Action* InteractiveStrategy::play_turn_internal(const Game& game,
                                                const Player& player) {
#if 0
    std::cout << player.get_name() << " turn :" << std::endl;
    // std::cout << *(game.get_buyable(0)->get_merchant()) << "\t\t" << std::endl;
    for(size_t i=0; i<Game::BUYABLE_COUNT; ++i) {
        const Buyable* buyable = game.get_buyable(i);
        std::cout << std::setw(2) << i << " ";
        std::cout << *(buyable->get_merchant()) << "  ";
        if (buyable->get_deposit().is_null()) {
            std::cout << "             ";
        } else {
            std::cout << buyable->get_deposit();
        }
        std::cout << "       |       ";
        if (i > 0) {
            std::cout << std::setw(2) << (i-1) << " ";
            std::cout << *(game.get_claimable(i-1)) ;
            if (i==1) {
                std::cout << " " << game.get_gold_coin_count()   << "GC";
            } else if (i==2) {
                std::cout << " " << game.get_silver_coin_count() << "SC";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    {
        std::cout << "Hand :" << std::endl;
        const std::unordered_set<const Merchant*>& hand = player.get_hand();
        if (hand.empty()) {
            std::cout << " X" << std::endl;
        } else {
            int i = 0; 
            for(const Merchant* card : hand) {
                std::cout << std::setw(2) << (i++) << " ";
                std::cout << (*card) << std::endl;
            }
        }
    }

    {
        std::cout << "Discard :" << std::endl;
        const std::unordered_set<const Merchant*>& discard = player.get_discard();
        if (discard.empty()) {
            std::cout << " X" << std::endl;
        } else {
            int i = 0;
            for(const Merchant* card : discard) {
                std::cout << std::setw(2) << (i++) << " ";
                std::cout << (*card) << std::endl;
            }
            std::cout << std::endl;
        }
    }

    {
        std::cout << "Caravan :" << std::endl;
        std::cout << player.get_caravan() << std::endl;
    }
#endif  //
    Action* result = nullptr;
    while (result == nullptr) {
        std::cout << "Choose : " << std::endl;
        std::cout << " 'c' : claim  | 'p' : play " << std::endl;
        std::cout << " 'b' : buy    | 's' : sleep" << std::endl;
        std::string line;
        getline(std::cin, line);
        char c = ' ';
        if (!line.empty()) {
            c = line[0];
        }
        switch (c) {
            case 'c':
                result = claim(game, player);
                break;
            case 'b':
                result = buy(game, player);
                break;
            case 'p':
                result = play(game, player);
                break;
            case 's':
                result = sleep(game, player);
                break;
            case 'q':
                exit(0);
                break;
            default:
                std::cout << "Unknown action : '" << c << "'" << std::endl;
                break;
        }
    }
    return result;
}

SpiceMap InteractiveStrategy::drop_spice(const Game& game,
                                         const Player& player) {
    SpiceMap result;
    // TODO
    return result;
}

Action* InteractiveStrategy::play(const Game& game, const Player& player) {
    if (player.get_hand().empty()) {
        std::cout << "Cannot play a card : empty hand" << std::endl;
        return nullptr;
    }

    std::cout << "Choose index of card in hand : " << std::endl;
    std::string line;
    getline(std::cin, line);
    unsigned int index = atoi(line.c_str());

    const std::unordered_set<const Merchant*>& hand = player.get_hand();
    std::vector<const Merchant*> hand_vector(hand.begin(), hand.end());
    if (index >= hand_vector.size()) {
        std::cout << "Cannot play a card : no card at index=" << index << " / "
                  << hand_vector.size() << std::endl;
        return nullptr;
    }
    const Merchant* card = hand_vector.at(index);
    std::cout << *card << std::endl;

    switch (card->get_type()) {
        case Merchant::Type::PRODUCTION: {
            std::cout << "Produce" << std::endl;
            return new ProduceAction(card->as_production());
        }
        case Merchant::Type::TRADE: {
            std::cout << "Trade how many times ?" << std::endl;
            getline(std::cin, line);
            unsigned int times = atoi(line.c_str());
            return new TradeAction(card->as_trade(), times);
        }
        case Merchant::Type::UPGRADE: {
            std::cout << "Upgrade what ? Y, R, G" << std::endl;
            SpiceMap upgrade_map;
            getline(std::cin, line);
            for (char c : line) {
                switch (c) {
                    case 'y':
                    case 'Y':
                        upgrade_map.remove(Spice::yellow);
                        upgrade_map.add(Spice::red);
                        break;
                    case 'r':
                    case 'R':
                        upgrade_map.remove(Spice::red);
                        upgrade_map.add(Spice::green);
                        break;
                    case 'g':
                    case 'G':
                        upgrade_map.remove(Spice::green);
                        upgrade_map.add(Spice::brown);
                        break;
                    default:
                        std::cout << "unknown spice : '" << c << "'"
                                  << std::endl;
                        return nullptr;
                }
            }
            return new UpgradeAction(card->as_upgrade(), upgrade_map);
        }
    }
    return nullptr;
}

Action* InteractiveStrategy::buy(const Game& game, const Player& player) {
    std::vector<Spice> cost;
    std::cout << "Cost ? Y, R, G, B" << std::endl;
    std::string line;
    getline(std::cin, line);
    for (char c : line) {
        switch (c) {
            case 'y':
            case 'Y':
                cost.push_back(Spice::yellow);
                break;
            case 'r':
            case 'R':
                cost.push_back(Spice::red);
                break;
            case 'g':
            case 'G':
                cost.push_back(Spice::green);
                break;
            case 'b':
            case 'B':
                cost.push_back(Spice::brown);
                break;
            default:
                std::cout << "unknown spice : '" << c << "'" << std::endl;
                break;
        }
    }
    SpiceMap cost_map(cost);
    std::cout << "cost : " << (cost_map.negate()) << std::endl;
    // TODO check that we can afford the cost

    return new BuyAction(cost);
}
Action* InteractiveStrategy::sleep(const Game& game, const Player& player) {
    return new SleepAction();
}
Action* InteractiveStrategy::claim(const Game& game, const Player& player) {
    std::cout << "Choose index of card in claim queue : " << std::endl;
    std::string line;
    getline(std::cin, line);
    unsigned int index = atoi(line.c_str());
    if (index >= game.get_claimables().size()) {
        std::cout << "Cannot claim a card : no card at index=" << index << " / "
                  << game.get_claimables().size() << std::endl;
        return nullptr;
    }
    return new ClaimAction(index);
}