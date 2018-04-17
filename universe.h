#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <Box2D.h>

#include <thread>

#include "space_ship.h"
#include "planet.h"

#include "object_manager.h"

class ShipContactListener : public b2ContactListener {
    void BeginContact(b2Contact * contact) {
        void * userData = 0;

        userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // TODO : Process contact begin (fixture A).
                std::cout << "Ship contact begin (a)\n";
            }
        }

        userData = contact->GetFixtureB()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // TODO : Process contact begin (fixture B).
                std::cout << "Ship contact begin (b)\n";
            }
        }
    }

    void EndContact(b2Contact * contact) {
        void * userData = 0;

        userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // TODO : Process contact end (fixture A).
                std::cout << "Ship contact end (a)\n";
            }
        }

        userData = contact->GetFixtureB()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // TODO : Process contact end (fixture B).
                std::cout << "Ship contact end (b)\n";
            }
        }
    }
};

class Universe {
public:
    Universe()
    : kNumPlanets(5)
    , world_(0)
    , space_ship_(0)
    , planets_(0)
    , state_(GameDefinitions::gameState_InMenu)
    {}
    ~Universe() {

        delete space_ship_;
        delete [] planets_;

        thread_.join();
    }

    void SetState(GameDefinitions::GameStateEnum state) {
        state_ = state;
    }

    void Init() {
        // Instantiate player ship.
        space_ship_ = new SpaceShip();
        space_ship_->SetPosition(0.0, 100.0);
        space_ship_->SetAngle(0.0);
        OBJMGR.Set("ship", space_ship_);

        // Instantiate planets.
        planets_ = new Planet[kNumPlanets];

        double u[][6] = {
            { 0.0, 0.0, 0.0, 100.0, 0.2, 435.0 }, // Earth
            { 600.0, 0.0, 0.0, 25.0, 0.01, 2730.0 }, // Moon
            { -19900.0, -11500.0, 0.0, 1000.0, 1.0, 3984.0 }, // Sun
            { -4900.0, -11500.0, 0.0, 90.0, 0.1, 1742.0 }, // Venus
            { 13000.0, -11500.0, 0.0, 50.0, 0.15, 3172.0 } //Mars
        };

        for (int i=0; i<kNumPlanets; ++i) {
            planets_[i].SetPosition(u[i][0], u[i][1]);
            planets_[i].SetAngle(u[i][2]);
            planets_[i].SetRadius(u[i][3]);
            planets_[i].SetAngularVelocity(u[i][4]);
            planets_[i].SetColor(u[i][5]);
        }

        OBJMGR.Set("planets", planets_);
        OBJMGR.Set("nplanets", (void *)&kNumPlanets);

        // Initialize physics.
        Box2DInit();

        // Initialize timer.
        t_begin_ = std::chrono::steady_clock::now();
        t_end_ = t_begin_;
    }
    void Box2DInit() {
        InitPhysics();

        // Planet creation
        InitPlanet();

        // Player creation
        InitPlayer();
    }
    void InitPhysics() {
        b2Vec2 gravity(0.0f, 0.0f);
        world_ = new b2World(gravity);
    }
    void InitPlanet() {

        for(int i=0; i<kNumPlanets; ++i) {
            planets_[i].Init(world_);
        }
    }
    void InitPlayer() {

        space_ship_->Init(world_);
    }
    void Run() {

        thread_ = std::thread(&Universe::ThreadLoop, this);
    }

private:
    void ThreadLoop() {
        while (true) {
            t_begin_ = std::chrono::steady_clock::now();
            double delta_time = std::chrono::duration<double> (t_begin_ - t_end_).count();

            UpdateGravity();

            UpdatePlayer(delta_time);

            UpdatePlanet();

            Step(delta_time);

            t_end_ = t_begin_;

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
    void UpdatePlayer(double delta_time) {

        space_ship_->Update(delta_time);
    }
    void UpdatePlanet() {

        for (int i=0; i<kNumPlanets; ++i) {
            planets_[i].Update();
        }
    }
    void Step(double delta_time) {
        // Advance physics
        if (state_ != GameDefinitions::gameState_InGame) {
            world_->Step(0.0, 12, 6);
        }
        else {
            world_->Step(delta_time, 12, 6);
        }
    }

private:
    const int kNumPlanets;
    std::chrono::time_point<std::chrono::steady_clock> t_begin_;
    std::chrono::time_point<std::chrono::steady_clock> t_end_;
    b2World * world_;

    std::thread thread_;
    SpaceShip * space_ship_;
    Planet * planets_;
    GameDefinitions::GameStateEnum state_;
};

#endif  // UNIVERSE_H_
