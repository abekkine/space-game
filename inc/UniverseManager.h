#ifndef UNIVERSE_MANAGER_H_
#define UNIVERSE_MANAGER_H_

#include "StorableObject.h"
#include "GameDefinitions.h"
#include "GameTimer.hpp"

#include <Box2D.h>

#include <thread>
#include <memory>

class SolarSystemInterface;
class SpaceShip;
class EffectsManager;
class CollisionHandler;
class UniverseInterface;

class UniverseManager : public StorableObject {
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
    std::shared_ptr<SolarSystemInterface> solar_system_;
    std::shared_ptr<SpaceShip> space_ship_;
    std::shared_ptr<EffectsManager> effects_;
    CollisionHandler * collision_handler_;
    GameDefinitions::GameStateEnum state_;

    GameTimer timer_;
    bool quit_;

    std::shared_ptr<UniverseInterface> universe_;
};

#endif  // UNIVERSE_MANAGER_H_
