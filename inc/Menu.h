#ifndef MENU_H_
#define MENU_H_

#include "Version.h"
#include "GameDefinitions.h"

#include <string>
#include <functional>

class Text;

// TODO : (#139) Separate menu item into a file.
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
    Menu();
    ~Menu();
    void Exit();
    void Init();
    void Render();
    GameDefinitions::GameStateEnum KeyInput(int key, bool action);

private:
    void InitMenuItems();
    void NextMenuItem();
    void PreviousMenuItem();
    GameDefinitions::GameStateEnum InvokeMenuItem();
    GameDefinitions::GameStateEnum ActionNewGame();
    GameDefinitions::GameStateEnum ActionContinue();
    GameDefinitions::GameStateEnum ActionSettings();
    GameDefinitions::GameStateEnum ActionQuit();

private:
    Text * text_;
    MenuItem * menu_items_[GameDefinitions::menuItem_SIZE];
    GameDefinitions::MenuItemEnum selected_item_;
    int screen_width_;
    int screen_height_;
    bool quit_;
};
#endif  // MENU_H_
