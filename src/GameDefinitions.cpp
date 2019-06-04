#include "GameDefinitions.h"

#include <math.h>

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

double LorentzFactor(double v) {
    double lf;
    double r = v / kSpeedOfLight;
    if (r > 1.0) {
        lf = kEpsilon;
    }
    else {
        lf = sqrt(1.0 - r*r);
    }
    return lf;
}

} // namespace GameDefinitions

