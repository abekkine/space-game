#ifndef GAME_H_
#define GAME_H_

#include "display.h"
#include "input.h"
#include "game_state.h"

class Game {
 public:
    Game()
    : input_(0)
    , game_state_(0)
    {}
    ~Game() {
        delete input_;
        delete game_state_;
    }
    void Init(int argc, char * argv[]) {
        (void)argc;
        (void)argv;

        // Scope for placeholders.
        {
            using std::placeholders::_1;
            DISPLAY.RegisterKeyCallback(std::bind(&Input::ProcessKey, input_, _1));
        }
        DISPLAY.Init();

        game_state_ = new GameState();
        game_state_->Init();
    }

    void Run() {
        while (!DISPLAY.QuitRequested()) {
            DISPLAY.PreRender();

            game_state_->Render();

            DISPLAY.PostRender();
        }

        DISPLAY.Quit();
    }

 private:
    std::function<void()> render_function_;
    Input * input_;
    GameState * game_state_;
};

#endif  // GAME_H_
