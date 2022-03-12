#include "Strategy.h"

#include <algorithm>
#include <iostream>

#include "Action.h"
#include "Game.h"
#include "Objective.h"
#include "Player.h"
#include "Spice.h"

using namespace sr;

BasicStrategy::BasicStrategy() : focus(nullptr) {}

Action* BasicStrategy::play_turn(const Game& game, const Player& player) {
    const SpiceMap& caravan = player.get_caravan();
    unsigned int focus_index;

#if 0
    // update the focused objective
    // if the strategy has no objective or if the current objetive has been claimed by another player
    if (focus == nullptr) {
        focus = select_objective(game, player);
        focus_index = game.index_of(focus);
    } else {
        // check if the previous focused objective is still avaible, if not select another objective
        const std::deque<const Objective*>& claimables = game.get_claimables();
        auto ite = std::find(claimables.begin(), claimables.end(), focus);
        if (ite == claimables.end()) {
            focus = select_objective(game, player);
            focus_index = game.index_of(focus);
        } else {
            focus_index = (ite - claimables.begin());
        }
    }
#else
    focus = select_objective(game, player);
    focus_index = game.index_of(focus);
#endif  //

    if (caravan.can_apply(focus->get_spice_map())) {
        return new ClaimAction(focus_index);
    }

    const std::unordered_set<const Merchant*>& hand = player.get_hand();

    Action* action = nullptr;
    unsigned int action_distance = focus->distance(player);
    unsigned int action_value = caravan.get_value();

    Action* action2 = nullptr;
    unsigned int action_distance2 = action_distance;

    // Test the buyable deposit to earn more
    unsigned int caravan_quantity = caravan.get_quantity();
    auto& buyables = game.get_buyables();
    for (unsigned int i = 0; i < buyables.size(); ++i) {
        if (caravan_quantity >= i) {
            const Buyable& buyable = buyables.at(i);
            SpiceMap drop_map =
                std::move(drop_spice(caravan, caravan_quantity - i));
            SpiceMap next_caravan(caravan);
            next_caravan.apply(drop_map);
            next_caravan.apply(buyable.get_deposit());
            SpiceMap drop_map_max = std::move(
                drop_spice(next_caravan, Player::MAX_SPICE_IN_CARAVAN));
            next_caravan.apply(drop_map_max);

            // the deposit can justify a buy by itself
            unsigned int next_distance = focus->distance(next_caravan);
            if (next_distance < action_distance) {
                delete action;
                action = new BuyAction(drop_map.to_vector(false));
                action_distance = next_distance;
            }

            // try to apply buyable.get_merchant() to the next_caravan, if it
            // makes earn even more than anything in hand : take it
            unsigned int next_distance2 = next_distance;
            Action* dummy_action = try_to_apply_all_combination(
                buyable.get_merchant(), next_caravan, next_distance2, nullptr);
            if (dummy_action != nullptr) {
                delete dummy_action;
                if (next_distance2 < action_distance2) {
                    delete action2;
                    action2 = new BuyAction(drop_map.to_vector(false));
                    action_distance2 = next_distance2;
                }
            }
        }
    }

    // Test all merchants in hand
    for (const Merchant* merchant : hand) {
        Action* new_action = try_to_apply_all_combination(
            merchant, player.get_caravan(), action_distance, &action_value);
        if (new_action != nullptr) {
            delete action;
            action = new_action;
        }
    }

    // Test all merchants in discard
    const std::unordered_set<const Merchant*>& discard = player.get_discard();
    for (const Merchant* merchant : discard) {
        Action* dummy_action = try_to_apply_all_combination(
            merchant, player.get_caravan(), action_distance2, nullptr);
        if (dummy_action != nullptr) {
            delete action2;
            action2 = new SleepAction();
            break;
        }
    }

    if (action != nullptr && action2 == nullptr) {
        return action;
    } else if (action == nullptr && action2 != nullptr) {
        return action2;
    } else if (action != nullptr && action2 != nullptr) {
        if (action_distance < action_distance2) {
            delete action2;
            return action;
        } else {
            delete action;
            return action2;
        }
    }

    if (player.get_discard().empty()) {
        std::cout << "uhoh... Local minimal... SleepAction call on an empty "
                     "discard pile..."
                  << std::endl;
    }

    return new SleepAction();
}

Action* BasicStrategy::try_to_apply_all_combination(
    const Merchant* merchant, const SpiceMap& caravan, unsigned int& distance,
    unsigned int* action_value) const {
    Action* action = nullptr;
    unsigned int action_distance = distance;
    // unsigned int action_value = caravan.get_value();
    switch (merchant->get_type()) {
        case Merchant::Type::PRODUCTION: {
            const Production* production = merchant->as_production();
            SpiceMap next_caravan(caravan);
            next_caravan.apply(production->get_spice_map());  // earn
            next_caravan.apply(
                drop_spice(next_caravan, Player::MAX_SPICE_IN_CARAVAN));
            unsigned int next_distance = focus->distance(next_caravan);
            unsigned int next_value = next_caravan.get_value();
            if ((next_distance < action_distance) ||
                ((next_distance == action_distance) &&
                 (action_value != nullptr) && (next_value > *action_value))) {
                delete action;
                action = new ProduceAction(production);
                action_distance = next_distance;
                if (action_value != nullptr) {
                    *action_value = next_value;
                }
            }
        } break;
        case Merchant::Type::UPGRADE: {
            const Upgrade* upgrade = merchant->as_upgrade();
            unsigned int upgrade_times = upgrade->get_upgrade_times();
            // for all possible combination of upgrade :
            std::vector<SpiceMap> all_possible_combination_of_upgrade(
                compute_all_upgrade_combination(upgrade_times, caravan));
            for (SpiceMap& upgrade_map : all_possible_combination_of_upgrade) {
                SpiceMap next_caravan(caravan);
                next_caravan.apply(upgrade_map);  // upgrade
                next_caravan.apply(
                    drop_spice(next_caravan, Player::MAX_SPICE_IN_CARAVAN));
                unsigned int next_distance = focus->distance(next_caravan);
                unsigned int next_value = next_caravan.get_value();
                if ((next_distance < action_distance) ||
                    ((next_distance == action_distance) &&
                     (action_value != nullptr) &&
                     (next_value > *action_value))) {
                    delete action;
                    action = new UpgradeAction(upgrade, upgrade_map);
                    action_distance = next_distance;
                    if (action_value != nullptr) {
                        *action_value = next_value;
                    }
                }
            }
        } break;
        case Merchant::Type::TRADE: {
            const Trade* trade = merchant->as_trade();
            const SpiceMap& trade_map = trade->get_spice_map();
            unsigned int max_times = compute_max_times_to_play(trade);
            for (unsigned int times = 1; times < max_times; ++times) {
                if (!caravan.can_apply(trade_map, times)) {
                    break;
                }
                SpiceMap next_caravan(caravan);
                next_caravan.apply(trade_map, times);  // upgrade
                next_caravan.apply(
                    drop_spice(next_caravan, Player::MAX_SPICE_IN_CARAVAN));
                unsigned int next_distance = focus->distance(next_caravan);
                unsigned int next_value = next_caravan.get_value();
                if ((next_distance < action_distance) ||
                    ((next_distance == action_distance) &&
                     (action_value != nullptr) &&
                     (next_value > *action_value))) {
                    delete action;
                    action = new TradeAction(trade, times);
                    action_distance = next_distance;
                    if (action_value != nullptr) {
                        *action_value = next_value;
                    }
                }
            }
        } break;
    };
    distance = action_distance;
    return action;
}

unsigned int BasicStrategy::compute_max_times_to_play(const Trade* trade) {
    unsigned int cumul = 0;
    const SpiceMap& trade_map = trade->get_spice_map();
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        cumul += std::max(trade_map.get_quantity((Spice)spice), 0);
    }
    if (cumul) {
        return Player::MAX_SPICE_IN_CARAVAN / cumul;
    } else {
        return 0;
    }
}

static void add_to_possible_upgrade_map(unsigned int upgrade_times,
                                        size_t first_spice,
                                        const SpiceMap& upgrade_map,
                                        const SpiceMap& caravan,
                                        std::vector<SpiceMap>& result) {
    for (size_t spice = first_spice; spice < (SPICE_TYPE_COUNT - 1); ++spice) {
        SpiceMap current_upgrade_map(upgrade_map);
        current_upgrade_map.remove((Spice)(spice));
        current_upgrade_map.add((Spice)(spice + 1));
        if (upgrade_times == 1) {
            if (caravan.can_apply(current_upgrade_map)) {
                result.push_back(current_upgrade_map);
            }
        } else {
            add_to_possible_upgrade_map(upgrade_times - 1, spice,
                                        current_upgrade_map, caravan, result);
        }
    }
}

std::vector<SpiceMap> BasicStrategy::compute_all_upgrade_combination(
    unsigned int max_upgrade_times, const SpiceMap& caravan) {
    std::vector<SpiceMap> result;
    for (unsigned int upgrade_times = 1; upgrade_times <= max_upgrade_times;
         ++upgrade_times) {
        SpiceMap empty_map;
        add_to_possible_upgrade_map(upgrade_times, 0, empty_map, caravan,
                                    result);
    }
    return result;
}

const Objective* BasicStrategy::select_objective(const Game& game,
                                                 const Player& player) {
    const Objective* focus = nullptr;
    unsigned int d_focus = UINT_MAX;
    const SpiceMap& caravan = player.get_caravan();
    const std::deque<const Objective*>& claimables = game.get_claimables();
    for (const Objective* o : claimables) {
        if (focus == nullptr) {
            focus = o;
            d_focus = o->distance(player);
        } else if (caravan.can_apply(o->get_spice_map())) {
            focus = o;
            // d_focus = 0; implicit
            break;
        } else {
            unsigned int d = o->distance(player);
            if ((d < d_focus) ||
                ((d == d_focus) && (focus->get_value() < o->get_value()))) {
                focus = o;
                d_focus = d;
            }
        }
    }
    return focus;
}

SpiceMap BasicStrategy::drop_spice(const Game& game, const Player& player) {
    SpiceMap drop_map = std::move(
        drop_spice(player.get_caravan(), Player::MAX_SPICE_IN_CARAVAN));
    return drop_map;
}

void BasicStrategy::build_all_possible_drop_map(
    std::set<SpiceMap>& result, const SpiceMap& caravan0,
    const SpiceMap& caravan, unsigned int quantity_to_drop) const {
    if (quantity_to_drop > 0) {
        for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
            Spice s = (Spice)spice;
            if (caravan.get_quantity(s) > 0) {
                SpiceMap next_caravan(caravan);
                next_caravan.remove(s);
                build_all_possible_drop_map(result, caravan0, next_caravan,
                                            quantity_to_drop - 1);
            }
        }
    } else {
        SpiceMap drop_map(caravan0);
        drop_map.apply(caravan.negate());
        result.emplace(drop_map.negate());
    }
}

SpiceMap BasicStrategy::drop_spice(const SpiceMap& caravan,
                                   unsigned int max_spice) const {
#if 1
    SpiceMap drop_map;
    // drop as many spice as necessary starting with the yellow, then the red,
    // etc...
    int remaining = caravan.get_quantity();
    size_t spice = 0;
    while (spice < SPICE_TYPE_COUNT && remaining > (int)max_spice) {
        Spice s = (Spice)spice;
        unsigned int q_to_remove_from_s =
            std::min(remaining - (int)max_spice, caravan.get_quantity(s));
        remaining -= q_to_remove_from_s;
        drop_map.remove(s, q_to_remove_from_s);
        ++spice;
    }
    return drop_map;
#else
    SpiceMap result;
    int quantity_to_drop = caravan.get_quantity() - max_spice;
    if (quantity_to_drop > 0) {
        std::set<SpiceMap> drop_maps;
        build_all_possible_drop_map(drop_maps, caravan, caravan,
                                    quantity_to_drop);
        unsigned int distance = UINT_MAX;  // focus->distance(caravan);
        for (const SpiceMap& drop_map : drop_maps) {
            SpiceMap next_caravan(caravan);
            next_caravan.apply(drop_map);
            unsigned int next_distance = focus->distance(next_caravan);
            if (next_distance < distance) {
                result = drop_map;
                distance = next_distance;
            }
        }
    }
    return result;
#endif  //
}

Action* InteractiveStrategy::play_turn(const Game& game, const Player& player) {
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
    while (true) {
        std::cout << "Choose : " << std::endl;
        std::cout << " 'c' : claim" << std::endl;
        std::cout << " 'b' : buy" << std::endl;
        std::cout << " 'p' : play" << std::endl;
        std::cout << " 's' : sleep" << std::endl;
        std::string line;
        getline(std::cin, line);
        char c = ' ';
        if (!line.empty()) {
            c = line[0];
        }
        switch (c) {
            case 'c':
                return claim(game, player);
            case 'b':
                return buy(game, player);
            case 'p':
                return play(game, player);
            case 's':
                return sleep(game, player);
            default:
                std::cout << "Unknown action : '" << c << "'" << std::endl;
        }
    }
    return nullptr;
}

SpiceMap InteractiveStrategy::drop_spice(const Game& game,
                                         const Player& player) {
    SpiceMap result;
    // TODO
    return result;
}

Action* InteractiveStrategy::play(const Game& game, const Player& player) {
    std::cout << "Choose index of card in hand : " << std::endl;
    std::string line;
    getline(std::cin, line);
    unsigned int index = atoi(line.c_str());

    const std::unordered_set<const Merchant*>& hand = player.get_hand();
    std::vector<const Merchant*> hand_vector(hand.begin(), hand.end());
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
                        break;
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
    return new ClaimAction(index);
}