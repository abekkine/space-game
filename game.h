#ifndef GAME_H_
#define GAME_H_

#include "display.h"
#include "input.h"
#include "game_render.h"

// TODO Implement game class +game +states id:2 gh:4 ic:gh
// - Add required methods to call within main.cpp
// - Add required (class) dependencies to make it work properly.
class Game {
 public:
    Game()
    : input_(0)
    , render_(0)
    {}
    ~Game() {
        delete input_;
        delete render_;
    }
    void Init(int argc, char * argv[]) {
        (void)argc;
        (void)argv;

        {
            using std::placeholders::_1;
            DISPLAY.RegisterKeyCallback(std::bind(&Input::ProcessKey, input_, _1));
        }
        DISPLAY.Init();

        render_ = new GameRender(true);
        render_->Init();
    }
    void Run() {
        while (!DISPLAY.QuitRequested()) {
            DISPLAY.PreRender();
            DISPLAY.WorldMode();
            render_->RenderWorld();
            DISPLAY.UiMode();
            render_->RenderUi();
            DISPLAY.PostRender();
        }

        DISPLAY.Quit();
    }

 private:
    Input * input_;
    GameRender * render_;
};

#endif  // GAME_H_
