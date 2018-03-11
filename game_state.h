#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <functional>

#include "menu.h"
#include "game_play.h"

class GameState {
 private:
    enum State {
        eInMenu,
        eInGame,
        eStateMax,
    };

 public:
    GameState()
    : state_(eInGame)
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

        render_states_[eInMenu] = std::bind(&Menu::Render, menu_);
        render_states_[eInGame] = std::bind(&GamePlay::Render, gameplay_);
    }
    void Render() {
        render_states_[state_]();
    }

 private:
    State state_;
    Menu * menu_;
    GamePlay * gameplay_;
    std::function<void()> render_states_[eStateMax];
};

#endif  // GAME_STATE_H_
