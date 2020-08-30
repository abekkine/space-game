#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include "GameDefinitions.h"

#include <memory>

class SpaceShip;
class UniverseManager;
class EffectsManager;
class Background;

class GamePlay {
private:
    double debug_scale_;
    bool quit_;
    double ship_angle_;
    double ship_x_;
    double ship_y_;
    double ship_speed_;
    double speed_scale_;
    EffectsManager* effects_;
    Background* background_;
    std::shared_ptr<UniverseManager> universe_mgr_;
    std::shared_ptr<SpaceShip> ship_;

public:
    GamePlay();
    ~GamePlay();
    void Exit();
    void Init();
    void RefreshPlayerParams();
    void Render();
    GameDefinitions::GameStateEnum KeyInput(int key, bool action);
};

#endif  // GAME_PLAY_H_
