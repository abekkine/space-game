#ifndef MENU_H_
#define MENU_H_

#include "Version.h"
#include "GameDefinitions.h"
#include "MenuItem.h"

#include <string>
#include <functional>

class Text;
class MenuItem;

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
