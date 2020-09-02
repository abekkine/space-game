#ifndef UNIVERSE_MANAGER_H_
#define UNIVERSE_MANAGER_H_

#include "StorableObject.h"
#include "GameDefinitions.h"
#include "GameTimer.hpp"
#include "UniverseInterface.h"

#include <Box2D/Box2D.h>

#include <thread>
#include <memory>

class SolarSystemInterface;
class SpaceShip;
class EffectsManager;
class CollisionHandler;
class MapView;

class UniverseManager : public StorableObject {
public:
    UniverseManager();
    ~UniverseManager();

    void SetState(GameDefinitions::GameStateEnum state);
    void Exit();
    void Init();
    void Run();
    void Render();
    void ToggleMap();
    void TogglePause();
    void SetSolarSystem(StarInterface * star);

private:
    void ThreadLoop();
    void UpdateMap();

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

    bool show_map_;
    bool paused_;
    StarCollectionType map_stars_;
    MapView * map_;
};

#endif  // UNIVERSE_MANAGER_H_
