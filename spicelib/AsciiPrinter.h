#ifndef __SR_ASCIIPRINTER_H__
#define __SR_ASCIIPRINTER_H__

#include <iostream>

namespace sr
{
    class Buyable;
    class Merchant;
    class Production;
    class Trade;
    class Upgrade;
    class Objective;
    class Game;
    class Player;

    class AsciiPrinter
    {
    public:
        AsciiPrinter();
        ~AsciiPrinter() {}

        std::ostream& print(std::ostream& stream, const Game& game);
        std::ostream& print(std::ostream& stream, const Player& player);

        std::ostream& print(std::ostream& stream, const Buyable* buyable, unsigned int line);
        std::ostream& print(std::ostream& stream, const Merchant*  merchant, unsigned int line);
        std::ostream& print(std::ostream& stream, const Objective* objective, unsigned int line);
    protected:
        std::ostream& _print(std::ostream& stream, const Production* production, unsigned int line);
        std::ostream& _print(std::ostream& stream, const Trade* trade, unsigned int line);
        std::ostream& _print(std::ostream& stream, const Upgrade* upgrade, unsigned int line);
    };
}

#endif // __SR_ASCIIPRINTER_H__
