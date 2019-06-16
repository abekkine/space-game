#ifndef UNIVERSE_MANAGER_H_
#define UNIVERSE_MANAGER_H_

#include "GameDefinitions.h"
#include "GameTimer.hpp"

#include <Box2D.h>

#include <thread>

class SolarSystemInterface;
class SpaceShip;
class EffectsManager;
class CollisionHandler;
class UniverseInterface;

class UniverseManager {
public:
    UniverseManager();
    ~UniverseManager();

    void SetState(GameDefinitions::GameStateEnum state);
    void Exit();
    void Init();
    void Run();
    void Render();

private:
    void ThreadLoop();

private:
    const int kThreadSleepDuration;
    b2World * world_;

    std::thread thread_;
    SolarSystemInterface * solar_system_;
    SpaceShip * space_ship_;
    EffectsManager * effects_;
    CollisionHandler * collision_handler_;
    GameDefinitions::GameStateEnum state_;

    GameTimer timer_;
    bool quit_;

    UniverseInterface * universe_;
};

#endif  // UNIVERSE_MANAGER_H_
