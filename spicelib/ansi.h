#ifndef __ANSI_COLOR_H__
#define __ANSI_COLOR_H__

#include <ostream>

namespace ansi
{
    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &reset(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[0m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &foreground_black(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[30m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &foreground_red(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[31m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &foreground_green(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[32m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &foreground_yellow(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[33m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &foreground_blue(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[34m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &foreground_magenta(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[35m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &foreground_cyan(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[36m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &foreground_white(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[37m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &bold_black(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[1m\033[30m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &bold_red(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[1m\033[31m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &bold_green(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[1m\033[32m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &bold_yellow(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[1m\033[33m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &bold_blue(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[1m\033[34m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &bold_magenta(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[1m\033[35m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &bold_cyan(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[1m\033[36m";
    }

    template <class CharT, class Traits>
    constexpr std::basic_ostream<CharT, Traits> &bold_white(std::basic_ostream<CharT, Traits> &os)
    {
        return os << "\033[1m\033[37m";
    }

} // ansi

#endif // __ANSI_COLOR_H__
