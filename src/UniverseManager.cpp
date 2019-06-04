#include "UniverseManager.h"

#include "SpaceShip.h"
#include "SolarSystemInterface.h"
#include "SolarSystemFactory.h"
#include "EffectsManager.h"
#include "ObjectManager.h"
#include "CollisionHandler.h"
#include "GameTimer.h"

#include <GLFW/glfw3.h>

UniverseManager::UniverseManager()
: world_(0)
, solar_system_(0)
, space_ship_(0)
, effects_(0)
, state_(GameDefinitions::gameState_InMenu)
, quit_(false)
{}

UniverseManager::~UniverseManager() {

    thread_.join();

    OBJMGR.Remove("effects");
    OBJMGR.Remove("ship");
    OBJMGR.Remove("solar");

    delete collision_handler_;
    delete solar_system_;
    delete space_ship_;
    delete effects_;
    delete world_;
}

void UniverseManager::SetState(GameDefinitions::GameStateEnum state) {
    state_ = state;
}

void UniverseManager::Exit() {
    quit_ = true;
}

void UniverseManager::Init() {
    // No gravity, since gravity will be modeled manually.
    b2Vec2 gravity(0.0f, 0.0f);
    world_ = new b2World(gravity);

    effects_ = new EffectsManager();
    effects_->Init();
    OBJMGR.Set("effects", effects_);

    // Instantiate player ship.
    space_ship_ = new SpaceShip();
    space_ship_->SetPosition(0.0, 110.0);
    space_ship_->SetAngle(0.0);
    OBJMGR.Set("ship", space_ship_);

    // Instantiate planets & solar system.
    solar_system_ = SolarSystemFactory::getTestingSystem();
    solar_system_->Init(world_);
    OBJMGR.Set("solar", solar_system_);

    // Player creation
    space_ship_->Init(world_);

    // Initialize physics.
    collision_handler_ = new CollisionHandler();
    world_->SetContactListener(collision_handler_);

    // Initialize timer.
    timer_.Init();
}

void UniverseManager::Run() {

    thread_ = std::thread(&UniverseManager::ThreadLoop, this);
}

void UniverseManager::Render() {

    if (quit_) return;

    solar_system_->Render();
}

void UniverseManager::ThreadLoop() {
    while (!quit_) {

        double delta_time = timer_.GetElapsed();

        space_ship_->Update(delta_time);

        solar_system_->Update(delta_time);

        effects_->Update(delta_time);

        // Advance physics
        if (state_ != GameDefinitions::gameState_InGame) {
            world_->Step(0.0, 12, 6);
        }
        else {
            world_->Step(delta_time, 12, 6);
        }

        // NOTE : Also, no magic numbers (10)?
        GameTimer::Sleep(10);
    }
}
