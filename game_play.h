#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include "display.h"
#include "game_render.h"

class GamePlay {
 public:
     GamePlay()
     : render_(0)
     {}
     ~GamePlay() {
         delete render_;
     }
     void Init() {
         if (render_ != 0) {
             delete render_;
         }
         render_ = new GameRender(true);
         render_->Init();
     }
     void Render() {
         DISPLAY.WorldMode();
         render_->Render();
     }

 private:
    GameRender * render_;
};

#endif  // GAME_PLAY_H_
