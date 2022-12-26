#ifndef __SR_MERCHANT_H__
#define __SR_MERCHANT_H__

#include "SpiceMap.h"
#include <iostream>

namespace sr
{
    class Production;
    class Upgrade;
    class Trade;

    class Merchant
    {
    public:
        enum class Type
        {
            PRODUCTION,
            UPGRADE,
            TRADE,
        };
        Type get_type() const {
            return type;
        }
        virtual ~Merchant() {}
        
        virtual const Production* as_production() const {
            return nullptr;
        }
        virtual const Trade* as_trade() const {
            return nullptr;
        }
        virtual const Upgrade* as_upgrade() const {
            return nullptr;
        }

        friend std::ostream& operator<< (std::ostream& stream, const Merchant& merchant);
    protected:
        virtual std::ostream& print(std::ostream& stream) const = 0;
    protected:
        Merchant(Type type);
        Type type;
    };

    std::ostream& operator<< (std::ostream& stream, const Merchant& merchant);

    class Production : public Merchant
    {
    public:
        Production(std::initializer_list<int> spice_map);
        ~Production() override {}

        const SpiceMap &get_spice_map() const
        {
            return spice_map;
        }

        const Production* as_production() const override {
            return this;
        }
    protected:
        std::ostream& print(std::ostream& stream) const override ;
    protected:
        SpiceMap spice_map;
    };

    class Trade : public Merchant
    {
    public:
        Trade(std::initializer_list<int> spice_map);
        ~Trade() override {}

        const SpiceMap &get_spice_map() const
        {
            return spice_map;
        }
        const Trade* as_trade() const override {
            return this;
        }
    protected:
        std::ostream& print(std::ostream& stream) const override ;
    protected:
        SpiceMap spice_map;
    };

    class Upgrade : public Merchant
    {
    public:
        Upgrade(unsigned int upgrade_times);
        ~Upgrade() override {}

        unsigned int get_upgrade_times() const {
            return upgrade_times;
        }
        const Upgrade* as_upgrade() const override {
            return this;
        }
    protected:
        std::ostream& print(std::ostream& stream) const override;
    protected:
        int upgrade_times;
    };

    class Buyable
    {
    public:
        explicit Buyable(const Merchant *merchant);
        ~Buyable();
        const Merchant *get_merchant() const;
        const SpiceMap &get_deposit() const;
        void add_deposit(const SpiceMap &new_deposit);
        void add_deposit(Spice spice);

    private:
        friend std::ostream& operator<< (std::ostream& stream, const Buyable& buyable);
    protected:
        const Merchant *merchant;
        SpiceMap deposit;
    };

    std::ostream& operator<< (std::ostream& stream, const Buyable& buyable);
}

#endif // __SR_MERCHANT_H__
