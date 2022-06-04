#include "NCursesRenderer.h"

#include <clocale>
#include <cstdlib>

#include "Merchant.h"
#include "Objective.h"
#include "Spice.h"

using namespace sr;

enum SpiceColorPair : short {
    SCP_YELLOW = 10,
    SCP_RED = 11,
    SCP_GREEN = 12,
    SCP_BROWN = 13,

    SCP_YELLOW_H = 20,
    SCP_RED_H = 21,
    SCP_GREEN_H = 22,
    SCP_BROWN_H = 23,
};

static void render(WINDOW* win, int y, int x, Spice spice) {
    static const cchar_t SPICE_CCHAR[] = {
        {0, L"■", SCP_YELLOW},
        {0, L"■", SCP_RED},
        {0, L"■", SCP_GREEN},
        {0, L"■", SCP_BROWN},
    };
    mvwadd_wch(win, y, x, SPICE_CCHAR + (int)spice);
}

static void render_white_spice(WINDOW* win, int y, int x) {
    static const cchar_t WHITE_SPICE_CCHAR = {0, L"■", 0};
    mvwadd_wch(win, y, x, &WHITE_SPICE_CCHAR);
}

static void render(WINDOW* win, const Objective& objective) {
    box(win, 0, 0);
    mvwaddch(win, 1, 2, 'O');
    std::vector<Spice> spices = objective.get_spice_map().to_vector(false);
    for (size_t i = 0; i < spices.size(); ++i) {
        render(win, 3 + i / 3, 2 * (1 + i % 3), spices[i]);
    }
    char buffer[16] = {0};
    sprintf(buffer, "%2u", objective.get_value());
    mvwaddstr(win, 6, 5, buffer);
    wrefresh(win);
}

static void render(WINDOW* win, const Merchant& merchant) {
    box(win, 0, 0);
    switch (merchant.get_type()) {
        case Merchant::Type::PRODUCTION: {
            mvwaddch(win, 1, 2, 'P');
            const Production* production = merchant.as_production();
            std::vector<Spice> spices_p =
                production->get_spice_map().to_vector(true);
            for (size_t i = 0; i < spices_p.size(); ++i) {
                render(win, 2 + i, 6, spices_p[i]);
            }
        } break;
        case Merchant::Type::TRADE: {
            mvwaddch(win, 1, 2, 'T');
            const Trade* trade = merchant.as_trade();
            std::vector<Spice> spices_n =
                trade->get_spice_map().to_vector(false);
            for (size_t i = 0; i < spices_n.size(); ++i) {
                render(win, 2 + i, 2, spices_n[i]);
            }
            std::vector<Spice> spices_p =
                trade->get_spice_map().to_vector(true);
            for (size_t i = 0; i < spices_p.size(); ++i) {
                render(win, 2 + i, 6, spices_p[i]);
            }

        } break;
        case Merchant::Type::UPGRADE: {
            mvwaddch(win, 1, 2, 'U');
            const Upgrade* upgrade = merchant.as_upgrade();
            unsigned int times = upgrade->get_upgrade_times();
            for (size_t i = 0; i < times; ++i) {
                render_white_spice(win, 2 + i, 4);
            }
        } break;
    }

    // for (size_t i = 0; i < spices.size(); ++i) {
    //     render(win, 3 + i / 3, 2 * (1 + i % 3), spices[i]);
    // }
    // char buffer[16] = {0};
    // sprintf(buffer, "%2u", objective.get_value());
    // mvwaddstr(win, 6, 5, buffer);
    wrefresh(win);
}

NCursesRenderer::NCursesRenderer() {
    // : claimables({nullptr, nullptr, nullptr, nullptr, nullptr}),
    //   buyables({nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}) {
    // init ncurses
    initscr();
    // Allow ctrl+C to break the application
    cbreak();
    // don't display what the user type
    noecho();
    keypad(stdscr, TRUE);

    // check if color are available
    has_color = has_colors();
    if (!has_color) {
        endwin();
        printf("Ncurses color are not available\n");
        exit(1);
    } else {
        start_color();

        init_pair(SCP_YELLOW, COLOR_YELLOW, COLOR_BLACK);
        init_pair(SCP_RED, COLOR_RED, COLOR_BLACK);
        init_pair(SCP_GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(SCP_BROWN, COLOR_MAGENTA, COLOR_BLACK);

        init_pair(SCP_YELLOW_H, COLOR_YELLOW, COLOR_WHITE);
        init_pair(SCP_RED_H, COLOR_RED, COLOR_WHITE);
        init_pair(SCP_GREEN_H, COLOR_GREEN, COLOR_WHITE);
        init_pair(SCP_BROWN_H, COLOR_MAGENTA, COLOR_WHITE);
    }

    static const int x_offset = 0;
    static const int y_offset = 0;

    for (size_t i = 0; i < std::size(claimables); ++i) {
        claimables[i] = newwin(8, 9, 0 + y_offset, 11 + i * 10 + x_offset);
    }
    for (size_t i = 0; i < std::size(buyables); ++i) {
        buyables[i] = newwin(8, 9, 8 + y_offset, 1 + i * 10 + x_offset);
    }

    // clear();
    refresh();
}
NCursesRenderer::~NCursesRenderer() {
    for (size_t i = 0; i < std::size(claimables); ++i) {
        delwin(claimables[i]);
    }
    for (size_t i = 0; i < std::size(buyables); ++i) {
        delwin(buyables[i]);
    }
    endwin();
}

void NCursesRenderer::render(const Game& game) {
    while (true) {
        for (size_t i = 0; i < std::size(claimables); ++i) {
            const Objective* objective = game.get_claimable(i);
            ::render(claimables[i], *objective);
        }
        for (size_t i = 0; i < std::size(buyables); ++i) {
            const Buyable* buyable = game.get_buyable(i);
            ::render(buyables[i], *(buyable->get_merchant()));
        }

        int choice = wgetch(stdscr);
        switch (choice) {
            case KEY_RESIZE:
                endwin();
                initscr();
                cbreak();
                noecho();
                continue;
            // case KEY_UP:
            // case KEY_LEFT:
            //     highlight--;
            //     break;
            // case KEY_DOWN:
            // case KEY_RIGHT:
            //     highlight++;
            //     break;
            default:
                break;
        }

        if (choice == '\n') {
            break;
        }
    }
}

void NCursesRenderer::render(const Player& player) {}
