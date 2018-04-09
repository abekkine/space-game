#include "game_definitions.h"

namespace GameDefinitions {

MenuItemEnum & operator++ (MenuItemEnum & m) {
    switch (m) {
        case menuItem_NewGame: return m = menuItem_Continue;
        case menuItem_Continue: return m = menuItem_Settings;
        case menuItem_Settings: return m = menuItem_Quit;
        case menuItem_Quit: return m = menuItem_Quit;
        default:
            return m;
            break;
    }
}

MenuItemEnum & operator-- (MenuItemEnum & m) {
    switch (m) {
        case menuItem_NewGame: return m = menuItem_NewGame;
        case menuItem_Continue: return m = menuItem_NewGame;
        case menuItem_Settings: return m = menuItem_Continue;
        case menuItem_Quit: return m = menuItem_Settings;
        default:
            return m;
            break;
    }
}

} // namespace GameDefinitions

