#include <windows.h>
#include "ansi.h"
#include "AsciiPrinter.h"
#include "Merchant.h"
#include "Objective.h"
#include "Game.h"
#include "Player.h"

using namespace sr;

struct _SpicePrint
{
    Spice spice;
};

inline _SpicePrint spice_color(Spice spice)
{
    return {spice};
}

template <class CharT, class Traits>
constexpr std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os, _SpicePrint spice_print)
{
    constexpr const char *colors[] = {
        "\033[33m",
        "\033[31m",
        "\033[32m",
        "\033[35m",
    };
    return os << colors[(int)spice_print.spice] << "■";
}

AsciiPrinter::AsciiPrinter()
{
    //SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleOutputCP(CP_UTF8);

    DWORD dwMode;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hOutput, &dwMode);
    dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING; 
    SetConsoleMode(hOutput, dwMode);
}

static std::ostream& print_item(std::ostream& stream, const std::vector<Spice>& v, unsigned int index) {
    if (v.size() > index) {
        return stream << spice_color(v[index]) << ansi::reset << " ";
    }
    return stream << "  ";
}

std::ostream& AsciiPrinter::print(std::ostream& stream, const Game& game) {
    stream << "           ";
    stream << "   " << std::setw(2) << ansi::foreground_yellow << game.get_gold_coin_count() << "©"<< ansi::reset;
    stream << "    ";
    stream << "   " << std::setw(2) << game.get_silver_coin_count() << "©";
    stream << std::endl;
    for (unsigned int i=0; i<8; ++i) {
        stream << " ";
        print(stream, (Objective*) nullptr, i);
        stream << " ";
        auto& claimables = game.get_claimables();
        for(const Objective* objective : claimables) {
            print(stream, objective, i);
            stream << " ";
        }
        stream << std::endl;
    }
    for (unsigned int i=0; i<10; ++i) {
        stream << " ";
        auto& buyables = game.get_buyables();
        for(const Buyable& buyable : buyables) {
            print(stream, &buyable, i);
            stream << " ";
        }
        stream << std::endl;
    }
    return stream;
}

std::ostream& AsciiPrinter::print(std::ostream& stream, const Player& player) {
    std::vector<Spice> v(player.get_caravan().to_vector(true));
    stream << " ┌─────────────┐" << std::endl;
    stream << " | C ";
    for(unsigned int i=0; i<5; ++i) {
        print_item(stream, v, i);
    }
    stream << "| OBJECTVES:  " << player.get_objectives().size() << std::endl;

    stream << " |   ";
    for(unsigned int i=5; i<10; ++i) {
        print_item(stream, v, i);
    }
    stream << "| SCORE:     " << std::setw(2) << player.get_score(false) << std::endl;
    stream << " └─────────────┘" << std::endl;

    stream << "Hand:" << std::endl;
    for (unsigned int i=0; i<8; ++i) {
        stream << " ";
        auto& hand = player.get_hand();
        for(const Merchant* merchant : hand) {
            print(stream, merchant, i);
            stream << " ";
        }
        stream << std::endl;
    }
    stream << "Discard pile:" << std::endl;
    for (unsigned int i=0; i<8; ++i) {
        stream << " ";
        auto& discard = player.get_discard();
        for(const Merchant* merchant : discard) {
            print(stream, merchant, i);
            stream << " ";
        }
        stream << std::endl;
    }
    return stream;
}

std::ostream& AsciiPrinter::print(std::ostream& stream, const Buyable* buyable, unsigned int line) {
    if (buyable == nullptr) {
        return stream << "         ";
    }
    if (line < 8) {
        return print(stream, buyable->get_merchant(), line);
    } else {
        unsigned int first_index = (line-8) * 3;
        std::vector<Spice> v(buyable->get_deposit().to_vector(true));
        stream << "  ";
        print_item(stream, v, first_index);
        print_item(stream, v, first_index + 1);
        print_item(stream, v, first_index + 2);
        stream << " ";
        return stream;
    }
}

std::ostream& AsciiPrinter::print(std::ostream& stream, const Merchant* merchant, unsigned int line) {
    if (merchant == nullptr) {
        return stream << "         ";
    }
    switch(merchant->get_type()) {
        case Merchant::Type::PRODUCTION:
            return _print(stream, merchant->as_production(), line);
        case Merchant::Type::TRADE:
            return _print(stream, merchant->as_trade(), line);
        case Merchant::Type::UPGRADE:
            return _print(stream, merchant->as_upgrade(), line);
        default:
            return stream << "         ";
    }
}

std::ostream &AsciiPrinter::_print(std::ostream &stream, const Production *production, unsigned int line)
{
    if (production == nullptr) {
        return stream << "         ";
    }
    std::vector<Spice> v(production->get_spice_map().to_vector(true));
    switch (line)
    {
    case 0:
        stream << "┌───────┐";
        break;
    case 1:
        stream << "| P     |";
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        stream << "|     ";
        print_item(stream, v, line-2);
        stream << "|";
        break;
    case 7:
        stream << "└───────┘";
        break;
    }
    return stream;
}

std::ostream& AsciiPrinter::_print(std::ostream& stream, const Trade* trade, unsigned int line) {
    if (trade == nullptr) {
        return stream << "         ";
    }
    std::vector<Spice> v_minus(trade->get_spice_map().to_vector(false));
    std::vector<Spice> v_plus (trade->get_spice_map().to_vector(true));
    switch (line)
    {
    case 0:
        stream << "┌───────┐";
        break;
    case 1:
        stream << "| T     |";
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        stream << "| ";
        print_item(stream, v_minus, line-2);
        stream<< "  ";
        print_item(stream, v_plus, line-2);
        stream << "|";
        break;
    case 7:
        stream << "└───────┘";
        break;
    }
    return stream;
}

std::ostream& AsciiPrinter::_print(std::ostream& stream, const Upgrade* upgrade, unsigned int line) {
    if (upgrade == nullptr) {
        return stream << "         ";
    }
    unsigned int times = upgrade->get_upgrade_times();
    switch (line)
    {
    case 0:
        stream << "┌───────┐";
        break;
    case 1:
        stream << "| U     |";
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (times > (line-2)) {
            stream << "|   ■   |";
        } else {
            stream << "|       |";
        }
        break;
    case 7:
        stream << "└───────┘";
        break;
    }
    return stream;
}

std::ostream& AsciiPrinter::print(std::ostream& stream, const Objective* objective, unsigned int line) {
    if (objective == nullptr) {
        return stream << "         ";
    }
    std::vector<Spice> v(objective->get_spice_map().to_vector(false));
    switch (line)
    {
    case 0:
        stream << "┌───────┐";
        break;
    case 1:
        stream << "| O     |";
        break;
    case 2:
    case 5:
        stream << "|       |";
        break;
    case 3:
    case 4:
        stream << "| ";
        print_item(stream, v, (line-3)*3);
        print_item(stream, v, (line-3)*3+1);
        print_item(stream, v, (line-3)*3+2);
        stream << "|";
        break;
    case 6:
        stream << "|    " << std::setw(2) << objective->get_value() << " |";
        break;
    case 7:
        stream << "└───────┘";
        break;
    }
    return stream;
}