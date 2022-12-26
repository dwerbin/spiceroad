#include "Merchant.h"

#include <iomanip>
#include <stdexcept>

#include "SpiceMap.h"

using namespace sr;

Merchant::Merchant(Type type) : type(type) {}

std::ostream& sr::operator<<(std::ostream& stream, const Merchant& merchant) {
    return merchant.print(stream);
}

Production::Production(std::initializer_list<int> spice_map)
    : Merchant(Type::PRODUCTION), spice_map(spice_map) {}

std::ostream& Production::print(std::ostream& stream) const {
    return stream << "{P," << spice_map << "}";
}

Trade::Trade(std::initializer_list<int> spice_map)
    : Merchant(Type::TRADE), spice_map(spice_map) {}

std::ostream& Trade::print(std::ostream& stream) const {
    return stream << "{T," << spice_map << "}";
}

Upgrade::Upgrade(unsigned int upgrade_times)
    : Merchant(Type::UPGRADE), upgrade_times(upgrade_times) {}

std::ostream& Upgrade::print(std::ostream& stream) const {
    return stream << "{U,{" << std::setw(14) << upgrade_times << "}}";
}

Buyable::Buyable(const Merchant* merchant) : merchant(merchant), deposit() {}

Buyable::~Buyable() {}

const SpiceMap& Buyable::get_deposit() const { return deposit; }

const Merchant* Buyable::get_merchant() const { return merchant; }

void Buyable::add_deposit(const SpiceMap& new_deposit) {
    if (!new_deposit.is_positive()) {
        throw std::runtime_error(
            "Cannot add a negative spice map to a buyable deposit");
    }
    deposit.apply(new_deposit);
}

void Buyable::add_deposit(Spice spice) { deposit.add(spice); }

std::ostream& sr::operator<<(std::ostream& stream, const Buyable& buyable) {
    return stream << "{" << buyable.deposit << ", " << *(buyable.merchant)
                  << "}";
}
