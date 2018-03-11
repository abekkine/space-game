#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <functional>

#include "game_definitions.h"
#include "menu.h"
#include "game_play.h"

class GameState {
 public:
    GameState()
    : state_(GameDefinitions::gameState_InMenu)
    , menu_(0)
    , gameplay_(0)
    {}
    ~GameState() {
        delete menu_;
        delete gameplay_;
    }
    void Init() {
        if (menu_ != 0) {
            delete menu_;
        }
        menu_ = new Menu();
        menu_->Init();

        if (gameplay_ != 0) {
            delete gameplay_;
        }
        gameplay_ = new GamePlay();
        gameplay_->Init();

        render_states_[GameDefinitions::gameState_InMenu] = std::bind(&Menu::Render, menu_);
        render_states_[GameDefinitions::gameState_InGame] = std::bind(&GamePlay::Render, gameplay_);

        key_processors_[GameDefinitions::gameState_InMenu] = std::bind(&Menu::KeyInput, menu_, std::placeholders::_1);
        key_processors_[GameDefinitions::gameState_InGame] = std::bind(&GamePlay::KeyInput, gameplay_, std::placeholders::_1);
    }
    void Render() {
        render_states_[state_]();
    }
    void ProcessKey(int key) {
        state_ = key_processors_[state_](key);
    }

 private:
    GameDefinitions::GameStateEnum state_;
    Menu * menu_;
    GamePlay * gameplay_;
    std::function<void()> render_states_[GameDefinitions::gameState_SIZE];
    std::function<GameDefinitions::GameStateEnum(int)> key_processors_[GameDefinitions::gameState_SIZE];
};

#endif  // GAME_STATE_H_
