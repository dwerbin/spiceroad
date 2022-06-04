#ifndef __SR_NCURSESRENDERER_H__
#define __SR_NCURSESRENDERER_H__

#define NCURSES_WIDECHAR 1
#include <ncursesw/ncurses.h>

#include "Game.h"
#include "Renderer.h"

namespace sr {
class NCursesRenderer : public Renderer {
   public:
    NCursesRenderer();
    ~NCursesRenderer() override;

    void render(const Game& game) override;
    void render(const Player& player) override;

   private:
    bool has_color;
    WINDOW* claimables[Game::CLAIMABLE_COUNT];
    WINDOW* buyables[Game::BUYABLE_COUNT];
};
}  // namespace sr

#endif  // __SR_NCURSESRENDERER_H__
