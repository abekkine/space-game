#include "Menu.h"
#include "Display.h"
#include "Text.h"

#include <GLFW/glfw3.h>

#include <iostream>

Menu::Menu()
: text_(0)
, menu_items_{}
, selected_item_(GameDefinitions::menuItem_NewGame)
, screen_width_(-1)
, screen_height_(-1)
, quit_(false)
{}
Menu::~Menu() {
    for (int i=0; i < GameDefinitions::menuItem_SIZE; ++i) {
        delete menu_items_[i];
    }
    delete text_;
}
void Menu::Exit() {
    quit_ = true;
}
void Menu::Init() {
    if (text_ != 0) {
        delete text_;
        text_ = 0;
    }

    text_ = new Text();
    text_->AddFont("menu", "../resources/FreeMono.ttf");
    text_->AddFont("version", "../resources/FreeMono.ttf");

    InitMenuItems();

    DISPLAY.GetSize(screen_width_, screen_height_);
}
void Menu::Render() {
    if (quit_) return;

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

    // TODO : Missing version info
    // std::string version_text = std::string(GIT_TAG);
    // int vpos_y = screen_height_ - 20;
    // int vpos_x = screen_width_ - (1 + version_text.length()) * 15;
    // text_->UseFont("version", 24);
    // text_->Render(vpos_x, vpos_y, std::string(VERSION_STR));
}
GameDefinitions::GameStateEnum Menu::KeyInput(int key, bool action) {
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

void Menu::InitMenuItems() {
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

void Menu::NextMenuItem() {
    if (selected_item_ < (GameDefinitions::menuItem_SIZE-1)) {
        ++selected_item_;
    }
}
void Menu::PreviousMenuItem() {
    if (selected_item_ > 0) {
        --selected_item_;
    }
}
GameDefinitions::GameStateEnum Menu::InvokeMenuItem() {
    MenuItem * activeMenu = menu_items_[selected_item_];

    return activeMenu->action();
}

GameDefinitions::GameStateEnum Menu::ActionNewGame() {
    return GameDefinitions::gameState_InGame;
}
GameDefinitions::GameStateEnum Menu::ActionContinue() {
    // TODO : (#142) Implement load and continue.
    return GameDefinitions::gameState_InGame;
}
GameDefinitions::GameStateEnum Menu::ActionSettings() {
    // TODO : (#141) Implement game settings.
    return GameDefinitions::gameState_InMenu;
}
GameDefinitions::GameStateEnum Menu::ActionQuit() {
    DISPLAY.RequestQuit();
    return GameDefinitions::gameState_InMenu;
}
