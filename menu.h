#ifndef MENU_H_
#define MENU_H_

#include <GLFW/glfw3.h>

#include "game_definitions.h"
#include "display.h"
#include "ui_render.h"

// TODO Implement Menu class +ui +input id:9 gh:11 ic:gh
// - Handles game menu.
class Menu {
 public:
    Menu()
    : ui_(0)
    {}
    ~Menu() {
        delete ui_;
    }
    void Init() {
        if (ui_ != 0) {
            delete ui_;
        }
        ui_ = new UiRender(true);
        ui_->Init();
    }
    void Render() {
        DISPLAY.UiMode();
        ui_->Render();
    }
    GameDefinitions::GameStateType KeyInput(int key) {
        GameDefinitions::GameStateType state = GameDefinitions::eInMenu;
        switch (key) {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                DISPLAY.RequestQuit();
                break;
            case GLFW_KEY_SPACE:
                state = GameDefinitions::eInGame;
                break;
        }

        return state;
    }

 private:
    UiRender * ui_;
};
#endif  // MENU_H_
