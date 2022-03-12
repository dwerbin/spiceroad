#include "Objective.h"
#include "Game.h"
#include "Action.h"
#include "Strategy.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace sr;


// Objective objectives[] = {
//     {6, {2, 2, 0, 0}},
//     {7, {3, 2, 0, 0}},
//     {8, {2, 3, 0, 0}},
//     {8, {2, 0, 2, 0}},
//     {8, {0, 4, 0, 0}},
//     {9, {2, 1, 0, 1}},
//     {9, {3, 0, 2, 0}},
//     {10, {2, 0, 0, 2}},
//     {10, {0, 2, 2, 0}},
//     {10, {0, 5, 0, 0}},
//     {11, {2, 0, 3, 0}},
//     {11, {3, 0, 0, 2}},
//     {12, {1, 1, 1, 1}},
//     {12, {1, 0, 2, 1}},
//     {12, {0, 2, 0, 2}},
//     {12, {0, 3, 2, 0}},
//     {12, {0, 2, 1, 1}},
//     {12, {0, 0, 4, 0}},
//     {13, {2, 2, 2, 0}},
//     {13, {0, 2, 3, 0}},
//     {14, {3, 1, 1, 1}},
//     {14, {2, 0, 0, 3}},
//     {14, {0, 3, 0, 2}},
//     {14, {0, 0, 2, 2}},
//     {15, {2, 2, 0, 2}},
//     {15, {0, 0, 5, 0}},
//     {16, {1, 3, 1, 1}},
//     {16, {0, 2, 0, 3}},
//     {16, {0, 0, 0, 4}},
//     {17, {2, 0, 2, 2}},
//     {17, {0, 0, 3, 2}},
//     {18, {1, 1, 3, 1}},
//     {18, {0, 0, 2, 3}},
//     {19, {0, 2, 2, 2}},
//     {20, {1, 1, 1, 3}},
//     {20, {0, 0, 0, 5}},
// };

// SpiceMap merchants[] = {
//     // production
//     {2, 0, 0, 0}, // starter card
//     {3, 0, 0, 0}, 
//     {4, 0, 0, 0}, 
//     {1, 1, 0, 0}, 
//     {2, 1, 0, 0}, 
//     {1, 0, 1, 0}, 
//     {0, 2, 0, 0}, 
//     {0, 0, 1, 0}, 
//     {0, 0, 0, 1}, 
//     // trade
//     {-2, 2, 0, 0}, 
//     {-2, 0, 1, 0}, 
//     {-3, 3, 0, 0}, 
//     {-3, 1, 1, 0}, 
//     {-3, 0, 0, 1}, 
//     {-4, 0, 2, 0}, 
//     {-4, 0, 1, 1}, 
//     {-5, 0, 3, 0}, 
//     {-5, 0, 0, 2}, 
//     {3, -1, 0, 0}, 
//     {3, -2, 1, 0}, 
//     {0, -2, 2, 0}, 
//     {2, -2, 0, 1}, 
//     {2, -3, 2, 0}, 
//     {0, -3, 3, 0}, 
//     {1, -3, 1, 1}, 
//     {0, -3, 0, 2}, 
//     {4, 1, -1, 0}, 
//     {1, 2, -1, 0}, 
//     {0, 2, -1, 0}, 
//     {2, 3, -2, 0}, 
//     {2, 1, -2, 1}, 
//     {0, 2, -2, 1}, 
//     {0, 0, -2, 2}, 
//     {0, 0, -3, 3}, 
//     {2, 2, 0, -1}, 
//     {0, 3, 0, -1}, 
//     {3, 0, 1, -1}, 
//     {1, 1, 1, -1}, 
//     {0, 0, 2, -1}, 
//     {1, 1, 3, -2}, 
//     {0, 3, 2, -2}, 
//     {-1, -1, 0, 1}, 
//     {-2, 0, -1, 2},
// };

// void infos_on_cards() {
//         std::cout << "Objectives : { " << std::endl;
//     for (const Objective& objective : objectives) {
//         std::cout << "\t" << objective << ", ";
//         unsigned int value = objective.get_value();
//         unsigned int true_value = objective.get_spice_map().get_value();
//         if (value != true_value) {
//             std::cout << "\t\t!!!!!!!!!!!! true_value=" << true_value << " (diff=" << value - true_value << ")" ;
//         }
//         std::cout << std::endl;
//     }
//     std::cout << "};" << std::endl;

//     std::cout << "Merchants : { " << std::endl;
//     for (const SpiceMap& merchant : merchants) {
//         std::cout << "\t" << "{" << merchant.get_value() << ", " << merchant << "}, " << std::endl;
//     }
//     std::cout << "};" << std::endl;
// }

int main(int argc, char **argv)
{
    for(unsigned int i=0; i<1000000; ++i) {
        std::cout << "Start Game #" << i << std::endl;
       // Player jean(std::string("Flo"), new InteractiveStrategy());
        Player robert(std::string("Robert"), new BasicStrategy());

        Game game;
        //game.add_player(jean);
        game.add_player(robert);

        game.prepare();
        game.execute();

        // display the points for each players
        std::vector<Player> players = game.get_players();
        std::sort(players.begin(), players.end(), 
            [&](const Player& p0, const Player& p1) {
                return (p0.get_score() < p1.get_score());
            }
        );
        for(const Player& player : players) {
            std::cout << player.get_name() << "\t" << player.get_objectives().size() << " objectives\t" << player.get_score() << " points"<< "\n"; 
        }
        std::cout << "End   Game #" << i << std::endl;
    }

    return 0;
}