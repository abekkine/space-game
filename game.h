#ifndef GAME_H_
#define GAME_H_

#include "display.h"
#include "game_state.h"
#include "universe.h"

class Game {
public:
    Game()
    : game_state_(0)
    , universe_(0)
    {}
    ~Game() {
        delete game_state_;
        delete universe_;
    }
    void Init(int argc, char * argv[]) {
        (void)argc;
        (void)argv;

        // Scope for placeholders.
        {
            using std::placeholders::_1;
            using std::placeholders::_2;
            DISPLAY.RegisterKeyCallback(std::bind(&Game::ProcessKey, this, _1, _2));
        }
        DISPLAY.Init();

        game_state_ = new GameState();
        game_state_->Init();

        // Start universe thread.
        universe_ = new Universe();
        universe_->Init();
    }

    void Run() {

        universe_->Run();

        while (!DISPLAY.QuitRequested()) {
            DISPLAY.PreRender();

            game_state_->Render();

            DISPLAY.PostRender();
        }

        DISPLAY.Quit();
    }

private:
    void ProcessKey(int key, bool action) {
        game_state_->ProcessKey(key, action);
    }

private:
    std::function<void()> render_function_;
    GameState * game_state_;
    Universe * universe_;
};

#endif  // GAME_H_
