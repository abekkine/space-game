#ifndef MENU_ITEM_H_
#define MENU_ITEM_H_

#include "GameDefinitions.h"

#include <string>
#include <functional>

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

#endif // MENU_ITEM_H_
