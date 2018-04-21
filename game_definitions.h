#ifndef GAME_DEFINITIONS_H_
#define GAME_DEFINITIONS_H_

// TODO : A shorter name for this namespace.
namespace GameDefinitions {

const double kEpsilon = 0.001;
const double kGravityConstant = 0.7;
const double kSpeedOfLight = 180.0;

enum OrbitDirEnum {
    od_Clockwise,
    od_CounterClockwise
};

enum GameStateEnum {
    gameState_InMenu,
    gameState_InGame,
    gameState_SIZE
};

enum MenuItemEnum {
    menuItem_NewGame,
    menuItem_Continue,
    menuItem_Settings,
    menuItem_Quit,
    menuItem_SIZE
};

MenuItemEnum & operator++ (MenuItemEnum & m);
MenuItemEnum & operator-- (MenuItemEnum & m);

double LorentzFactor(double v);

}  // namespace GameDefinitions

#endif  // GAME_DEFINITIONS_H_
