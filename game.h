#ifndef GAME_H_
#define GAME_H_

#include "display.h"
#include "input.h"
#include "game_render.h"
#include "ui_render.h"

class Game {
 private:
    enum GameState {
        eNone = 0,
        eInMenu,
        eInGame,
    };

 public:
    Game()
    : input_(0)
    , game_state_(eNone)
    , render_(0)
    , ui_(0)
    {}
    ~Game() {
        delete input_;
        delete render_;
        delete ui_;
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

        ui_ = new UiRender(true);
        ui_->Init();

        game_state_ = eInMenu;
    }

    void RenderMenu() {
        // Render Ui
        DISPLAY.UiMode();
        ui_->Render();
    }

    void RenderGame() {
        // Render world
        DISPLAY.WorldMode();
        render_->Render();
    }

    void Run() {
        while (!DISPLAY.QuitRequested()) {
            DISPLAY.PreRender();

            switch (game_state_) {
                case eInMenu:
                    RenderMenu();
                    break;
                case eInGame:
                    RenderGame();
                    break;
                default:
                    break;
            }

            DISPLAY.PostRender();
        }

        DISPLAY.Quit();
    }

 private:
    Input * input_;
    GameState game_state_;
    GameRender * render_;
    UiRender * ui_;
};

#endif  // GAME_H_
