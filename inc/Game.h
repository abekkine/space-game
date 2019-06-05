#ifndef GAME_H_
#define GAME_H_

#include "GameDefinitions.h"

#include <functional>

class GamePlay;
class Menu;
class UniverseManager;

class Game {
public:
    Game();
    ~Game();
    void Init(int argc, char * argv[]);
    void Run();

private:
    void ProcessKey(int key, bool action);

private:
    std::function<void()> render_function_;
    GameDefinitions::GameStateEnum state_;
    UniverseManager * universe_mgr_;
    Menu * menu_;
    GamePlay * game_play_;
    std::function<void()> render_states_[GameDefinitions::gameState_SIZE];
    std::function<GameDefinitions::GameStateEnum(int, bool)> key_processors_[GameDefinitions::gameState_SIZE];
};

#endif  // GAME_H_
