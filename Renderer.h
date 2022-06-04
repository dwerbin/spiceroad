#ifndef __SR_RENDERER_H__
#define __SR_RENDERER_H__

namespace sr {

class Game;
class Player;

class Renderer {
   public:
    Renderer() {}
    virtual ~Renderer() {}
    virtual void render(const Game& game) = 0;
    virtual void render(const Player& player) = 0;
};

}  // namespace sr

#endif  // __SR_RENDERER_H__
