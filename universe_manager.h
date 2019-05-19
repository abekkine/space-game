#ifndef UNIVERSE_MANAGER_H_
#define UNIVERSE_MANAGER_H_

#include <GLFW/glfw3.h>
#include <Box2D.h>

#include <thread>

#include "space_ship.h"
#include "solar_system_interface.h"
#include "solar_system_factory.h"
#include "effects_manager.h"
#include "object_manager.h"
#include "collision_handler.h"
#include "game_timer.h"

class UniverseManager {
public:
    UniverseManager()
    : world_(0)
    , solar_system_(0)
    , space_ship_(0)
    , effects_(0)
    , state_(GameDefinitions::gameState_InMenu)
    , quit_(false)
    {}
    ~UniverseManager() {

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

    void SetState(GameDefinitions::GameStateEnum state) {
        state_ = state;
    }

    void Exit() {
        quit_ = true;
    }

    void Init() {
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
    void Run() {

        thread_ = std::thread(&UniverseManager::ThreadLoop, this);
    }
    void Render() {

        if (quit_) return;

        solar_system_->Render();
    }

private:
    void ThreadLoop() {
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

private:
    b2World * world_;

    std::thread thread_;
    SolarSystemInterface * solar_system_;
    SpaceShip * space_ship_;
    EffectsManager * effects_;
    CollisionHandler * collision_handler_;
    GameDefinitions::GameStateEnum state_;

    GameTimer timer_;
    bool quit_;
};

#endif  // UNIVERSE_MANAGER_H_
