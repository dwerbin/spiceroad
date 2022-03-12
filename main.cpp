#include <algorithm>
#include <iostream>
#include <string>

#include "Action.h"
#include "Game.h"
#include "Objective.h"
#include "Strategy.h"

using namespace sr;

int main(int argc, char** argv) {
    for (unsigned int i = 0; i < 1000000; ++i) {
        std::cout << "Start Game #" << i << std::endl;
        // Player jean(std::string("Flo"), new InteractiveStrategy());
        Player robert(std::string("Robert"), new BasicStrategy());

        Game game;
        // game.add_player(jean);
        game.add_player(robert);

        game.prepare();
        game.execute();

        // display the points for each players
        std::vector<Player> players = game.get_players();
        std::sort(players.begin(), players.end(),
                  [&](const Player& p0, const Player& p1) {
                      return (p0.get_score() < p1.get_score());
                  });
        for (const Player& player : players) {
            std::cout << player.get_name() << "\t"
                      << player.get_objectives().size() << " objectives\t"
                      << player.get_score() << " points"
                      << "\n";
        }
        std::cout << "End   Game #" << i << std::endl;
    }

    return 0;
}