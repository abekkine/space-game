#ifndef GAME_H_
#define GAME_H_

#include <thread>

#include "game_definitions.h"
#include "display.h"
#include "game_play.h"
#include "menu.h"
#include "universe.h"
#include "object_manager.h"
#include "game_timer.h"

class Game {
public:
    Game()
    : render_function_(0)
    , state_(GameDefinitions::gameState_InMenu)
    , universe_(0)
    , menu_(0)
    , game_play_(0)
    , render_states_{0}
    , key_processors_{0}
    {}
    ~Game() {
        delete game_play_;
        delete menu_;
        delete universe_;
    }
    void Init(int argc, char * argv[]) {
        (void)argc;
        (void)argv;

        DISPLAY.Init();

        // Start universe thread.
        universe_ = new Universe();
        universe_->Init();
        OBJMGR.Set("universe", universe_);

        menu_ = new Menu();
        menu_->Init();

        game_play_ = new GamePlay();
        game_play_->Init();

        {
            using std::placeholders::_1;
            using std::placeholders::_2;
            key_processors_[GameDefinitions::gameState_InMenu] = std::bind(&Menu::KeyInput, menu_, _1, _2);
            key_processors_[GameDefinitions::gameState_InGame] = std::bind(&GamePlay::KeyInput, game_play_, _1, _2);
            render_states_[GameDefinitions::gameState_InMenu] = std::bind(&Menu::Render, menu_);
            render_states_[GameDefinitions::gameState_InGame] = std::bind(&GamePlay::Render, game_play_);
            DISPLAY.RegisterKeyCallback(std::bind(&Game::ProcessKey, this, _1, _2));
        }
    }

    void Run() {

        universe_->Run();

        while (!DISPLAY.QuitRequested()) {

            universe_->SetState(state_);

            DISPLAY.PreRender();

            render_states_[state_]();

            DISPLAY.PostRender();

            GameTimer::Sleep(10);
        }

        DISPLAY.Quit();
    }

private:
    void ProcessKey(int key, bool action) {
        state_ = key_processors_[state_](key, action);
    }

private:
    std::function<void()> render_function_;
    GameDefinitions::GameStateEnum state_;
    Universe * universe_;
    Menu * menu_;
    GamePlay * game_play_;
    std::function<void()> render_states_[GameDefinitions::gameState_SIZE];
    std::function<GameDefinitions::GameStateEnum(int, bool)> key_processors_[GameDefinitions::gameState_SIZE];
};

#endif  // GAME_H_
