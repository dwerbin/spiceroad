#include "SpiceMap.h"

#include <cstring>
#include <iomanip>
#include <memory>
#include <numeric>
#include <stdexcept>

using namespace sr;

SpiceMap::SpiceMap() { memset(quantities, 0, SPICE_TYPE_COUNT * sizeof(int)); }

SpiceMap::SpiceMap(std::initializer_list<int> that_quantities) {
    memcpy(quantities, that_quantities.begin(),
           std::min(SPICE_TYPE_COUNT, that_quantities.size()) * sizeof(int));
}

SpiceMap::SpiceMap(const std::vector<Spice>& spice_vector) {
    memset(quantities, 0, SPICE_TYPE_COUNT * sizeof(int));
    for (Spice spice : spice_vector) {
        add(spice);
    }
}

SpiceMap::SpiceMap(const SpiceMap& that) {
    memcpy(quantities, that.quantities, SPICE_TYPE_COUNT * sizeof(int));
}

SpiceMap::~SpiceMap() {}

SpiceMap& SpiceMap::operator=(const SpiceMap& that) {
    memcpy(quantities, that.quantities, SPICE_TYPE_COUNT * sizeof(int));
    return *this;
}

bool SpiceMap::operator==(const SpiceMap& that) const {
    for (unsigned int s = 0; s < SPICE_TYPE_COUNT; ++s) {
        if (quantities[s] != that.quantities[s]) {
            return false;
        }
    }
    return true;
}

bool SpiceMap::operator<(const SpiceMap& that) const {
    for (unsigned int s = SPICE_TYPE_COUNT - 1; s >= 0; --s) {
        if (s && quantities[s] == that.quantities[s]) {
            continue;
        }
        return quantities[s] < that.quantities[s];
    }
    return false;
}

int SpiceMap::get_quantity(Spice spice) const {
    return quantities[static_cast<int>(spice)];
}

int SpiceMap::get_quantity() const {
    int value = 0;
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        value += quantities[spice];
    }
    return value;
}

int SpiceMap::get_value() const {
    int value = 0;
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        value += quantities[spice] * (spice + 1);
    }
    return value;
}

bool SpiceMap::is_null() const {
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        if (quantities[spice] != 0) {
            return false;
        }
    }
    return true;
}

bool SpiceMap::is_positive() const {
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        if (quantities[spice] < 0) {
            return false;
        }
    }
    return true;
}

bool SpiceMap::is_negative() const {
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        if (quantities[spice] > 0) {
            return false;
        }
    }
    return true;
}

SpiceMap SpiceMap::negate() const {
    SpiceMap copy;
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        copy.quantities[spice] = -quantities[spice];
    }
    return copy;
}

void SpiceMap::add(Spice spice, unsigned int quantity) {
    quantities[static_cast<int>(spice)] += quantity;
}

void SpiceMap::remove(Spice spice, unsigned int quantity) {
    quantities[static_cast<int>(spice)] -= quantity;
}

void SpiceMap::upgrade(Spice spice, unsigned int times) {
    if (can_upgrade(spice, times)) {
        remove(spice);
        Spice upgraded_spice = (Spice)(static_cast<int>(spice) + times);
        add(upgraded_spice);
    } else {
        throw std::runtime_error("Cannot upgrade a spice");
    }
}

void SpiceMap::upgrade(const SpiceMap& upgrade_map) {
    if (!upgrade_map.is_positive()) {
        throw std::runtime_error("Cannot upgrade a spice");
    }
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        int loop = upgrade_map.quantities[spice];
        for (int i = 0; i < loop; ++i) {
            upgrade((Spice)spice, 1);
        }
    }
}

void SpiceMap::apply(const SpiceMap& that, unsigned int times,
                     bool check_positive) {
    int new_quantities[SPICE_TYPE_COUNT] = {
        0,
        0,
        0,
        0,
    };
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        int tmp = quantities[spice] + that.quantities[spice] * times;
        if (!check_positive || tmp < 0) {
            throw std::runtime_error(
                "Cannot apply a spice map to another : the destination space "
                "map should stay positive");
        }
        new_quantities[spice] = tmp;
    }
    memcpy(quantities, new_quantities, SPICE_TYPE_COUNT * sizeof(int));
}

int SpiceMap::sum() const {
    return std::accumulate(quantities, quantities + SPICE_TYPE_COUNT, 0,
                           std::plus<int>());
}

bool SpiceMap::can_upgrade(Spice spice, unsigned int times) const {
    return (is_upgradable(spice) >= times && get_quantity(spice) > 0);
}

bool SpiceMap::can_apply(const SpiceMap& that, unsigned int times) const {
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        int tmp = quantities[spice] + that.quantities[spice] * times;
        if (tmp < 0) {
            return false;
        }
    }
    return true;
}

std::vector<Spice> SpiceMap::to_vector(bool positive) const {
    std::vector<Spice> result;
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        int q = quantities[spice];
        if (!positive) {
            q = -q;
        }
        for (int i = 0; i < q; ++i) {
            result.push_back((Spice)spice);
        }
    }
    return result;
}

unsigned int sr::distance(const SpiceMap& o, const SpiceMap& c) {
    unsigned int result = 0;
    for (size_t spice = 0; spice < SPICE_TYPE_COUNT; ++spice) {
        int diff = std::max(o.quantities[spice] - c.quantities[spice], 0);
        result += diff * (spice + 1);
    }
    return result;
}

std::ostream& sr::operator<<(std::ostream& stream, const SpiceMap& spice_map) {
    return stream << "{" << std::setw(2) << spice_map.quantities[0] << ", "
                  << std::setw(2) << spice_map.quantities[1] << ", "
                  << std::setw(2) << spice_map.quantities[2] << ", "
                  << std::setw(2) << spice_map.quantities[3] << "}";
}
