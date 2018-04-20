#ifndef MENU_H_
#define MENU_H_

#include <GLFW/glfw3.h>

#include <string>
#include <functional>

#include "game_definitions.h"
#include "display.h"
#include "text.h"

#include "version.h"

struct MenuItem {
    explicit MenuItem(std::string l, int x, int y, std::function<GameDefinitions::GameStateEnum()> func)
    : label(l)
    , position_x(x)
    , position_y(y)
    , action(func)
    {}

    std::string label;
    int position_x, position_y;
    std::function<GameDefinitions::GameStateEnum()> action;
};

class Menu {
public:
    Menu()
    : text_(0)
    , menu_items_{}
    , selected_item_(GameDefinitions::menuItem_NewGame)
    , screen_width_(-1)
    , screen_height_(-1)
    {}
    ~Menu() {
        for (int i=0; i < GameDefinitions::menuItem_SIZE; ++i) {
            delete menu_items_[i];
        }
    }
    void Init() {
        if (text_ != 0) {
            delete text_;
            text_ = 0;
        }

        text_ = new Text();
        text_->AddFont("menu", "fonts/menu_font.ttf");
        text_->AddFont("version", "fonts/mono/FreeMono.ttf");

        InitMenuItems();

        DISPLAY.GetSize(screen_width_, screen_height_);
    }
    void Render() {
        DISPLAY.UiMode();

        text_->UseFont("menu", 40);

        for (int i=0; i < GameDefinitions::menuItem_SIZE; ++i) {
            if (i == selected_item_) {
                glColor3f(1.0, 0.0, 0.0);
            } else {
                glColor3f(1.0, 1.0, 1.0);
            }

            text_->Render(menu_items_[i]->position_x, menu_items_[i]->position_y, menu_items_[i]->label);
        }

        std::string version_text = std::string(VERSION_STR);
        int vpos_y = screen_height_ - 20;
        int vpos_x = screen_width_ - (1 + version_text.length()) * 15;
        text_->UseFont("version", 24);
        text_->Render(vpos_x, vpos_y, std::string(VERSION_STR));
    }
    GameDefinitions::GameStateEnum KeyInput(int key, bool action) {
        GameDefinitions::GameStateEnum state = GameDefinitions::gameState_InMenu;
        if (action == true)
        switch (key) {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                DISPLAY.RequestQuit();
                break;
            // case GLFW_KEY_SPACE:
            //     state = GameDefinitions::gameState_InGame;
            //     break;
            case GLFW_KEY_UP:
                PreviousMenuItem(); break;
            case GLFW_KEY_DOWN:
                NextMenuItem(); break;
            case GLFW_KEY_ENTER:
            case GLFW_KEY_RIGHT:
                state = InvokeMenuItem(); break;
            default:
                std::cout << "Menu Key : [" << key << "]\n";
                break;
        }

        return state;
    }

private:
    void InitMenuItems() {
        MenuItem * item = 0;

        item = new MenuItem("New Game", 100, 100, std::bind(&Menu::ActionNewGame, this));
        menu_items_[GameDefinitions::menuItem_NewGame] = item;

        item = new MenuItem("Continue", 100, 200, std::bind(&Menu::ActionContinue, this));
        menu_items_[GameDefinitions::menuItem_Continue] = item;

        item = new MenuItem("Settings", 100, 300, std::bind(&Menu::ActionSettings, this));
        menu_items_[GameDefinitions::menuItem_Settings] = item;

        item = new MenuItem("Quit", 100, 400, std::bind(&Menu::ActionQuit, this));
        menu_items_[GameDefinitions::menuItem_Quit] = item;
    }

    void NextMenuItem() {
        if (selected_item_ < (GameDefinitions::menuItem_SIZE-1)) {
            ++selected_item_;
        }
    }
    void PreviousMenuItem() {
        if (selected_item_ > 0) {
            --selected_item_;
        }
    }
    GameDefinitions::GameStateEnum InvokeMenuItem() {
        MenuItem * activeMenu = menu_items_[selected_item_];

        return activeMenu->action();
    }

    GameDefinitions::GameStateEnum ActionNewGame() {
        return GameDefinitions::gameState_InGame;
    }
    GameDefinitions::GameStateEnum ActionContinue() {
        std::cout << "[TODO] Continue selected\n";
        return GameDefinitions::gameState_InGame;
    }
    GameDefinitions::GameStateEnum ActionSettings() {
        std::cout << "[TODO] Settings selected\n";
        return GameDefinitions::gameState_InMenu;
    }
    GameDefinitions::GameStateEnum ActionQuit() {
        DISPLAY.RequestQuit();
        return GameDefinitions::gameState_InMenu;
    }

private:
    Text * text_;
    MenuItem * menu_items_[GameDefinitions::menuItem_SIZE];
    GameDefinitions::MenuItemEnum selected_item_;
    int screen_width_;
    int screen_height_;
};
#endif  // MENU_H_
