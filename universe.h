#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <GLFW/glfw3.h>
#include <Box2D.h>

#include <thread>

#include "space_ship.h"
#include "planet.h"
#include "effects_manager.h"
#include "object_manager.h"
#include "collision_handler.h"

class Universe {
public:
    Universe()
    : kNumPlanets(5)
    , world_(0)
    , space_ship_(0)
    , planets_(0)
    , effects_(0)
    , state_(GameDefinitions::gameState_InMenu)
    {}
    ~Universe() {

        // [TODO] : Not all allocated objects are being deleted.
        //        : + effects_
        //        : + world_
        //        : + collision_handler_
        delete space_ship_;
        delete [] planets_;

        thread_.join();
    }

    void SetState(GameDefinitions::GameStateEnum state) {
        state_ = state;
    }

    struct CelestialBodyDef {
        double x, y;
        double angle;
        double radius;
        double coreRadius;
        double angularVelocity;
        double color;
    };
    void Init() {
        effects_ = new EffectsManager();
        effects_->Init();
        OBJMGR.Set("effects", effects_);

        // Instantiate player ship.
        space_ship_ = new SpaceShip();
        space_ship_->SetPosition(0.0, 100.0);
        space_ship_->SetAngle(0.0);
        OBJMGR.Set("ship", space_ship_);

        // Instantiate planets.
        planets_ = new Planet[kNumPlanets];

        CelestialBodyDef u[] = {
            {      0.0,      0.0, 0.0,  100.0,  99.7,  0.20,  435.0 }, // Earth
            {    600.0,      0.0, 0.0,   25.0,  24.9,  0.01, 2730.0 }, // Moon
            { -19900.0, -11500.0, 0.0, 1000.0, 100.0,  1.00, 3984.0 }, // Sun
            {  -4900.0, -11500.0, 0.0,   90.0,  89.9,  0.10, 1742.0 }, // Venus
            {  13000.0, -11500.0, 0.0,   50.0,  49.9,  0.15, 3172.0 }  // Mars
        };

        for (int i=0; i<kNumPlanets; ++i) {
            planets_[i].SetPosition(u[i].x, u[i].y);
            planets_[i].SetAngle(u[i].angle);
            planets_[i].SetRadius(u[i].radius);
            planets_[i].SetCoreRadius(u[i].coreRadius);
            planets_[i].SetAngularVelocity(u[i].angularVelocity);
            planets_[i].SetColor(u[i].color);
        }

        // [TODO] : Planets & numPlanets would be passed inside a single object.
        OBJMGR.Set("planets", planets_);
        OBJMGR.Set("nplanets", (void *)&kNumPlanets);

        // Initialize physics.
        // No gravity, since gravity will be modeled manually.
        b2Vec2 gravity(0.0f, 0.0f);
        world_ = new b2World(gravity);
        collision_handler_ = new CollisionHandler();
        world_->SetContactListener(collision_handler_);

        // Planet creation
        for(int i=0; i<kNumPlanets; ++i) {
            planets_[i].Init(world_);
        }

        // Player creation
        space_ship_->Init(world_);

        // [TODO] : It would be convenient to encapsulate timer.
        // Initialize timer.
        t_begin_ = std::chrono::steady_clock::now();
        t_end_ = t_begin_;
    }
    void Run() {

        thread_ = std::thread(&Universe::ThreadLoop, this);
    }
    void Render() {
        for (int i=0; i<kNumPlanets; ++i) {
            glPushMatrix();
            planets_[i].Render();
            glPopMatrix();
        }
    }

private:
    void ThreadLoop() {
        while (true) {
            // [TODO] : Timer encapsulation?
            t_begin_ = std::chrono::steady_clock::now();
            double delta_time = std::chrono::duration<double> (t_begin_ - t_end_).count();

            UpdateGravity();

            space_ship_->Update(delta_time);

            for (int i=0; i<kNumPlanets; ++i) {
                planets_[i].Update();
            }

            effects_->Update(delta_time);

            // Advance physics
            if (state_ != GameDefinitions::gameState_InGame) {
                world_->Step(0.0, 12, 6);
            }
            else {
                world_->Step(delta_time, 12, 6);
            }

            t_end_ = t_begin_;

            // [TODO] : Encapsulate with timer stuff?
            // [TODO] : Also, no magic numbers (10)?
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    void UpdateGravity() {
        // Gravity
        b2Vec2 g = b2Vec2(0.0, 0.0);
        for (int i=0; i<kNumPlanets; ++i) {

            g += planets_[i].GetGravityAcceleration(space_ship_->GetPosition());
        }
        space_ship_->SetGravityAcceleration(g);
    }

private:
    const int kNumPlanets;
    std::chrono::time_point<std::chrono::steady_clock> t_begin_;
    std::chrono::time_point<std::chrono::steady_clock> t_end_;
    b2World * world_;

    std::thread thread_;
    SpaceShip * space_ship_;
    Planet * planets_;
    EffectsManager * effects_;
    CollisionHandler * collision_handler_;
    GameDefinitions::GameStateEnum state_;
};

#endif  // UNIVERSE_H_
