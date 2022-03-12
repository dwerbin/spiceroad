#ifndef __SR_SPICEMAP_H__
#define __SR_SPICEMAP_H__

#include <map>
#include <vector>
#include <ostream>
#include "Spice.h"

namespace sr
{
    class SpiceMap
    {
    public:
        static const SpiceMap null;
    public:
        SpiceMap();
        SpiceMap(std::initializer_list<int> that_quantities);
        explicit SpiceMap(const std::vector<Spice> &spice_vector);
        SpiceMap(const SpiceMap& that);
        ~SpiceMap();

        SpiceMap& operator=(const SpiceMap& that);
        bool operator==(const SpiceMap& that) const;
        bool operator<(const SpiceMap& that) const;

        int get_quantity(Spice spice) const;
        int get_quantity() const;
        int get_value() const;
        bool is_null() const;
        bool is_positive() const;
        bool is_negative() const;
        SpiceMap negate() const;

        std::vector<Spice> to_vector(bool positive) const;

        void add(Spice spice, unsigned int quantity = 1);
        void remove(Spice spice, unsigned int quantity = 1);
        void upgrade(Spice spice, unsigned int times = 1); // used by play -> upgrade
        void upgrade(const SpiceMap& upgrade_map);
        void apply(const SpiceMap& that, unsigned int times = 1, bool check_positive = true); // used by play -> production / trade
        int sum() const;

        bool can_upgrade(Spice spice, unsigned int times = 1) const;
        bool can_apply(const SpiceMap& that, unsigned int times = 1) const;

        friend unsigned int distance(const SpiceMap& a, const SpiceMap& b);
        friend std::ostream& operator<< (std::ostream& stream, const SpiceMap& spice_map);

    protected:
        int quantities[SPICE_TYPE_COUNT];
    };

    unsigned int distance(const SpiceMap& a, const SpiceMap& b);
    std::ostream& operator<< (std::ostream& stream, const SpiceMap& spice_map);
}

#endif // __SR_SPICEMAP_H__
