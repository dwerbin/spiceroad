#include "Player.h"

#include <numeric>

#include "Action.h"
#include "Strategy.h"

using namespace sr;

Player::Player(const std::string& name, Strategy* strategy)
    : name(name),
      strategy(strategy),
      caravan(),
      hand(),
      discard_pile(),
      objectives(),
      gold_coins(0),
      silver_coins(0),
      starter_upgrade(2),
      starter_production({
          2,
          0,
          0,
          0,
      }) {
    hand.insert(&starter_upgrade);
    hand.insert(&starter_production);
}

Player::~Player() {}

Action* Player::play_turn(const Game& game) const {
    if (strategy) {
        return strategy->play_turn(game, *this);
    } else {
        throw std::runtime_error("Cannot play the turn : strategy is missing");
    }
}

void Player::play(const Production& card) {
    auto ite = check_merchant_card_in_hand(card);
    caravan.apply(card.get_spice_map());  // earn
    discard(ite);
}

void Player::play(const Trade& card, unsigned int times) {
    auto ite = check_merchant_card_in_hand(card);
    caravan.apply(card.get_spice_map(), times);  // pay + earn
    discard(ite);
}
void Player::play(const Upgrade& card, const SpiceMap& upgrade_map) {
    auto ite = check_merchant_card_in_hand(card);
    if (((int)card.get_upgrade_times()) >= upgrade_map.get_value() &&
        upgrade_map.sum() == 0) {
        caravan.apply(upgrade_map);  // upgrade
        // no drop is needed
    } else {
        throw std::runtime_error("Cannot upgrate spices : bad upgrade map");
    }
    discard(ite);
}

void Player::buy(const Buyable& buyable, const std::vector<Spice>& cost) {
    SpiceMap cost_map(cost);
    caravan.apply(cost_map.negate());
    caravan.apply(buyable.get_deposit());  // earn
    hand.insert(buyable.get_merchant());
}

void Player::sleep() {
    hand.insert(discard_pile.begin(), discard_pile.end());
    discard_pile.clear();
}

void Player::claim(const Objective& objective) {
    caravan.apply(objective.get_spice_map());  // pay
    objectives.insert(&objective);
}

void Player::drop_spice_if_needed(const Game& game) {
    if (caravan.get_quantity() > MAX_SPICE_IN_CARAVAN) {
        if (strategy) {
            SpiceMap drop_map(strategy->drop_spice(game, *this));
            if (!drop_map.is_negative()) {
                throw std::runtime_error(
                    "Cannot drop spice : non negative drop map");
            } else if (!caravan.can_apply(drop_map)) {
                throw std::runtime_error(
                    "Cannot drop spice : cannot apply the drop map to the "
                    "caravan");
            } else {
                caravan.apply(drop_map);
            }
        } else {
            throw std::runtime_error("Cannot drop spice : strategy is missing");
        }

        if (caravan.get_quantity() > MAX_SPICE_IN_CARAVAN) {
            throw std::runtime_error(
                "Too many spice in caravan after a drop of spice");
        }
    }
}

std::unordered_set<const Merchant*>::iterator
Player::check_merchant_card_in_hand(const Merchant& card) {
    std::unordered_set<const Merchant*>::iterator ite = hand.find(&card);
    if (ite == hand.end()) {
        throw std::runtime_error(
            "Cannot play a merchant card wich is not in the hand of the "
            "player");
    }
    return ite;
}

void Player::discard(
    std::unordered_set<const Merchant*>::iterator hand_iterator) {
    discard_pile.insert(*hand_iterator);
    hand.erase(hand_iterator);
}

unsigned int Player::get_score(bool with_spice_bonuses) const {
    unsigned int score = std::accumulate(
        objectives.begin(), objectives.end(), 0UL,
        [](unsigned int s, const Objective* o) { return s + o->get_value(); });
    score += gold_coins * GOLD_COIN_SCORE;
    score += silver_coins * SILVER_COIN_SCORE;

    if (with_spice_bonuses) {
        score += caravan.get_quantity(Spice::red);
        score += caravan.get_quantity(Spice::green);
        score += caravan.get_quantity(Spice::brown);
    }
    return score;
}

void Player::add_gold_coin() { gold_coins++; }

void Player::add_silver_coin() { silver_coins++; }
