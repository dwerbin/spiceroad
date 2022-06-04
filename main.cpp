#include <algorithm>
#include <iostream>
#include <string>

#include "Action.h"
#include "BasicStrategy.h"
#include "Game.h"
#include "InteractiveStrategy.h"
#include "NCursesRenderer.h"
#include "Objective.h"
#include "Timer.h"

using namespace sr;

void show_results(std::ostream& stream, const Game& game) {
    // display the points for each players
    std::vector<Player> players = game.get_players();
    std::sort(players.begin(), players.end(),
              [&](const Player& p0, const Player& p1) {
                  return (p0.get_score() < p1.get_score());
              });
    for (const Player& player : players) {
        stream.precision(2);
        stream << player.get_name() << "\t" << player.get_objectives().size()
               << " objectives\t" << game.get_round_id() + 1 << " turns\t"
               << player.get_score() << " points\t" << std::fixed
               << ((double)player.get_score()) /
                      ((double)(game.get_round_id() + 1))
               << " points per turns\t"
               << "\n";
    }
}

/*
int main(int argc, char** argv) {
    for (unsigned int i = 0; i < 1; ++i) {
        std::cout << "Start Game #" << i << std::endl;
        Game game;

        // Player p1(std::string("Flo"), new InteractiveStrategy());
        // game.add_player(p1);

        Player p2(std::string("Robert"), new BasicStrategy());
        game.add_player(p2);

        game.prepare();
        Timer t;
        game.execute();
        std::cout << t.duration().count() << "ms" << std::endl;

        show_results(std::cout, game);
        std::cout << "End   Game #" << i << std::endl;
    }

    return 0;
}
*/

int main(int argc, char** argv) {
    Game game;

    Player p1(std::string("Flo"), new InteractiveStrategy());
    game.add_player(p1);

    Player p2(std::string("Robert"), new BasicStrategy());
    game.add_player(p2);

    game.prepare();
    NCursesRenderer renderer;
    renderer.render(game);

    game.execute();

    show_results(std::cout, game);

    return 0;
}
