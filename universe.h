#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <GLFW/glfw3.h>
#include <Box2D.h>

#include <thread>

#include "space_ship.h"
#include "solar_system.h"
#include "effects_manager.h"
#include "object_manager.h"
#include "collision_handler.h"
#include "game_timer.h"

class Universe {
public:
    Universe()
    : world_(0)
    , solar_system_(0)
    , space_ship_(0)
    , effects_(0)
    , state_(GameDefinitions::gameState_InMenu)
    {}
    ~Universe() {

        // [TODO] : Not all allocated objects are being deleted.
        //        : + effects_
        //        : + world_
        //        : + collision_handler_
        //        : + solar_system_
        delete space_ship_;

        thread_.join();
    }

    void SetState(GameDefinitions::GameStateEnum state) {
        state_ = state;
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
        space_ship_->SetPosition(0.0, 100.0);
        space_ship_->SetAngle(0.0);
        OBJMGR.Set("ship", space_ship_);

        // Instantiate planets & solar system.
        solar_system_ = new SolarSystem();
        solar_system_->Init(world_);

        // Player creation
        space_ship_->Init(world_);

        // Initialize physics.
        collision_handler_ = new CollisionHandler();
        world_->SetContactListener(collision_handler_);

        // Initialize timer.
        timer_.Init();
    }
    void Run() {

        thread_ = std::thread(&Universe::ThreadLoop, this);
    }
    void Render() {
        solar_system_->Render();
    }

private:
    void ThreadLoop() {
        while (true) {

            double delta_time = timer_.GetElapsed();

            UpdateGravity();

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

            // TODO : Also, no magic numbers (10)?
            GameTimer::Sleep(10);
        }
    }
    void UpdateGravity() {
        // Gravity
        b2Vec2 g = solar_system_->GetGravityAcceleration(space_ship_->GetPosition());
        space_ship_->SetGravityAcceleration(g);
    }

private:
    b2World * world_;

    std::thread thread_;
    SolarSystem * solar_system_;
    SpaceShip * space_ship_;
    EffectsManager * effects_;
    CollisionHandler * collision_handler_;
    GameDefinitions::GameStateEnum state_;

    GameTimer timer_;
};

#endif  // UNIVERSE_H_
