#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include <GLFW/glfw3.h>

#include "game_definitions.h"
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
     GameDefinitions::GameStateEnum KeyInput(int key) {
         GameDefinitions::GameStateEnum state = GameDefinitions::gameState_InGame;

         switch (key) {
             case GLFW_KEY_Q:
             case GLFW_KEY_ESCAPE:
                state = GameDefinitions::gameState_InMenu;
                break;
         }

         return state;
     }

 private:
    GameRender * render_;
};

#endif  // GAME_PLAY_H_
