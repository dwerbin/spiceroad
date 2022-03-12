#include "Action.h"
#include "Player.h"
#include "Game.h"
#include "Merchant.h"
#include "Objective.h"

using namespace sr;

void SleepAction::execute(Game &game, Player &player)
{
    player.sleep();
}
bool SleepAction::valid(Game& game, Player& player) const {
    return !(player.get_discard().empty());
}
std::ostream& SleepAction::print(std::ostream& stream) const {
    return stream << "[S] {}";
}

void ClaimAction::execute(Game &game, Player &player)
{
    const Objective *objective = game.get_claimable(claimable_index);
    player.claim(*objective);
    game.pop_claimable(claimable_index);

    if (claimable_index == 0 && game.get_gold_coin_count() > 0)
    {
        player.add_gold_coin();
        game.remove_gold_coin();
    }
    else if (claimable_index == 0 && game.get_silver_coin_count() > 0)
    {
        player.add_silver_coin();
        game.remove_silver_coin();
    }
    else if (claimable_index == 1 && game.get_silver_coin_count() > 0)
    {
        player.add_silver_coin();
        game.remove_silver_coin();
    }
}
bool ClaimAction::valid(Game& game, Player& player) const {
    bool result = (claimable_index < Game::CLAIMABLE_COUNT);
    if (result) {
        const Objective *objective = game.get_claimable(claimable_index);
        result = player.get_caravan().can_apply(objective->get_spice_map());
    }
    return result;
}
std::ostream& ClaimAction::print(std::ostream& stream) const {
    return stream << "[C] {" << claimable_index << "}";
}

void BuyAction::execute(Game &game, Player &player) {
    unsigned int buyable_index = cost.size();
    const Buyable *buyable = game.get_buyable(buyable_index);
    player.buy(*buyable, cost);
    game.add_deposit(cost);
    game.pop_buyable(buyable_index);
}
bool BuyAction::valid(Game& game, Player& player) const {
    unsigned int buyable_index = cost.size();
    bool result = (buyable_index < Game::BUYABLE_COUNT);
    if (result) {
        SpiceMap cost_map(cost);
        result = player.get_caravan().can_apply(cost_map.negate());
    }
    return result;
}
std::ostream& BuyAction::print(std::ostream& stream) const {
    stream << "[B] {";
    for(Spice spice : cost) {
        switch(spice) {
            case Spice::yellow:
                stream << 'Y';
                break;
            case Spice::red:
                stream << 'R';
                break;
            case Spice::green:
                stream << 'G';
                break;
            case Spice::brown:
                stream << 'B';
                break;
            default:
                stream << '?';
                break;
        }
    }
    stream << "}";
    return stream;
}

void ProduceAction::execute(Game &game, Player &player) {
    player.play(*card);
}
bool ProduceAction::valid(Game& game, Player& player) const {
    return player.get_hand().find(card) != player.get_hand().end();
}
std::ostream& ProduceAction::print(std::ostream& stream) const {
    return stream << "[P] {" << *card << "}";
}

void TradeAction::execute(Game &game, Player &player) {
    player.play(*card, times);
}
bool TradeAction::valid(Game& game, Player& player) const {
    return player.get_hand().find(card) != player.get_hand().end() &&
        player.get_caravan().can_apply(card->get_spice_map(), times);
}
std::ostream& TradeAction::print(std::ostream& stream) const {
    return stream << "[T] {" << *card << ", " << times << "}";
}

void UpgradeAction::execute(Game &game, Player &player)
{
    player.play(*card, upgrade_map);
}
bool UpgradeAction::valid(Game& game, Player& player) const {
    return player.get_hand().find(card) != player.get_hand().end() &&
        player.get_caravan().can_apply(upgrade_map);
}
std::ostream& UpgradeAction::print(std::ostream& stream) const {
    return stream << "[U] {" << *card << ", " << upgrade_map << "}";
}