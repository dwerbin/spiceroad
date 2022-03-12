#ifndef __SR_ACTION_H__
#define __SR_ACTION_H__

#include <vector>

#include "Spice.h"
#include "SpiceMap.h"

namespace sr
{
    class Game;
    class Player;
    class Objective;
    class Buyable;
    class Production;
    class Trade;
    class Upgrade;

    class Action
    {
    public:
        virtual ~Action() {}
        virtual void execute(Game &game, Player &player) = 0;
        virtual bool valid(Game &game, Player &player) const = 0;

        friend std::ostream& operator<< (std::ostream& stream, const Action& action) {
            return action.print(stream);
        }
    protected:
        virtual std::ostream& print(std::ostream& stream) const = 0;
    };

    class SleepAction : public Action
    {
    public:
        SleepAction() : 
        Action() {
        }
        ~SleepAction() override {
        }
        void execute(Game &game, Player &player) override;
        bool valid(Game &game, Player &player) const override;
    protected:
        std::ostream& print(std::ostream& stream) const override;
    };

    class ClaimAction : public Action
    {
    public:
        explicit ClaimAction(unsigned int claimable_index) : Action(),
                                                    claimable_index(claimable_index)
        {
        }
        ~ClaimAction() override { }
        void execute(Game &game, Player &player) override;
        bool valid(Game &game, Player &player) const override;

        unsigned int get_claimable_index() const {
            return claimable_index;
        }
    protected:
        std::ostream& print(std::ostream& stream) const override;
    private:
        unsigned int claimable_index;
    };

    class BuyAction : public Action
    {
    public:
        explicit BuyAction(const std::vector<Spice> &cost) : 
            Action(),
            cost(cost) {
        }
        ~BuyAction() override { }
        void execute(Game &game, Player &player) override;
        bool valid(Game &game, Player &player) const override;

        const std::vector<Spice>& get_cost() const {
            return cost;
        }
    protected:
        std::ostream& print(std::ostream& stream) const override;
    private:
        std::vector<Spice> cost;
    };

    class ProduceAction : public Action
    {
    public:
        explicit ProduceAction(const Production *card) : 
            Action(),
            card(card) {
        }
        ~ProduceAction() override { }
        void execute(Game &game, Player &player) override;
        bool valid(Game &game, Player &player) const override;

        const Production* get_card() const {
            return card;
        }
    protected:
        std::ostream& print(std::ostream& stream) const override;
    private:
        const Production* card;
    };

    class TradeAction : public Action
    {
    public:
        TradeAction(const Trade *card, unsigned int times) : 
            Action(),
            card(card), 
            times(times) {
        }
        ~TradeAction() override {
        }
        void execute(Game &game, Player &player) override;
        bool valid(Game &game, Player &player) const override;

        const Trade* get_card() const {
            return card;
        }
        unsigned int get_times() const {
            return times;
        }
    protected:
        std::ostream& print(std::ostream& stream) const override;
    private:
        const Trade *card;
        unsigned int times;
    };

    class UpgradeAction : public Action
    {
    public:
        UpgradeAction(const Upgrade *card, const SpiceMap &upgrade_map) : 
            Action(),
            card(card),
            upgrade_map(upgrade_map) {
        }
        ~UpgradeAction() override {
        }
        void execute(Game &game, Player &player) override;
        bool valid(Game &game, Player &player) const override;
        const Upgrade* get_card() const {
            return card;
        }
        const SpiceMap& get_upgrade_map() const {
            return upgrade_map;
        }
    protected:
        std::ostream& print(std::ostream& stream) const override;
    private:
        const Upgrade *card;
        const SpiceMap upgrade_map;
    };
}

#endif // __SR_ACTION_H__
