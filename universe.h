#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <Box2D.h>

#include <thread>

#include "space_ship.h"
#include "planet.h"
#include "effects_manager.h"

#include "object_manager.h"

// [TODO] : Contact listener would be moved into a separate file,
//        : and would be renamed w/o 'Ship' prefix, since it listens
//        : for all contacts in physics world.
//        : If, only ship contacts will be processed here, name
//        : would stay, however if other body contacts need to be
//        : processed, name should change.
class ShipContactListener : public b2ContactListener {

    void PostSolve(b2Contact * contact, const b2ContactImpulse* impulse) {

        void * userData = 0;
        userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // Process impulse
                int count = contact->GetManifold()->pointCount;
                float maxImpulse = 0.0;
                for (int i=0; i<count; ++i) {
                    maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
                }

                ship->ProcessImpulse(maxImpulse);
            }
        }
        else {
            std::cout << "userData is '0', maybe check fixture B?\n";
        }
    }

    void BeginContact(b2Contact * contact) {
        void * userData = 0;

        userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // Process contact begin (fixture A).
                ship->BeginContact();
            }
        }
        else {
            std::cout << "userData is '0', maybe check fixture B?\n";
        }
    }

    void EndContact(b2Contact * contact) {
        void * userData = 0;

        userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // Process contact end (fixture A).
                ship->EndContact();
            }
        }
        else {
            std::cout << "userData is '0', mayba check fixture B?\n";
        }
    }
} shipContacts;

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
        delete space_ship_;
        delete [] planets_;

        thread_.join();
    }

    void SetState(GameDefinitions::GameStateEnum state) {
        state_ = state;
    }

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

        // [TODO] : It would be better to use a readable struct for
        //        : planet's initialization.
        double u[][7] = {
            {      0.0,      0.0, 0.0,  100.0,  99.7,  0.20,  435.0 }, // Earth
            {    600.0,      0.0, 0.0,   25.0,  24.9,  0.01, 2730.0 }, // Moon
            { -19900.0, -11500.0, 0.0, 1000.0, 100.0,  1.00, 3984.0 }, // Sun
            {  -4900.0, -11500.0, 0.0,   90.0,  89.9,  0.10, 1742.0 }, // Venus
            {  13000.0, -11500.0, 0.0,   50.0,  49.9,  0.15, 3172.0 }  // Mars
        };

        for (int i=0; i<kNumPlanets; ++i) {
            planets_[i].SetPosition(u[i][0], u[i][1]);
            planets_[i].SetAngle(u[i][2]);
            planets_[i].SetRadius(u[i][3]);
            planets_[i].SetCoreRadius(u[i][4]);
            planets_[i].SetAngularVelocity(u[i][5]);
            planets_[i].SetColor(u[i][6]);
        }

        // [TODO] : Planets & numPlanets would be passed inside a single object.
        OBJMGR.Set("planets", planets_);
        OBJMGR.Set("nplanets", (void *)&kNumPlanets);

        // Initialize physics.
        Box2DInit();

        // [TODO] : It would be convenient to encapsulate timer.
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

        // No gravity, since gravity will be modeled manually.
        b2Vec2 gravity(0.0f, 0.0f);
        world_ = new b2World(gravity);

        world_->SetContactListener(&shipContacts);
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
            // [TODO] : Timer encapsulation?
            t_begin_ = std::chrono::steady_clock::now();
            double delta_time = std::chrono::duration<double> (t_begin_ - t_end_).count();

            UpdateGravity();

            UpdatePlayer(delta_time);

            UpdatePlanet();

            UpdateEffects(delta_time);

            Step(delta_time);

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
    // [TODO] : Consider removing functions for single line calls.
    void UpdatePlayer(double delta_time) {

        space_ship_->Update(delta_time);
    }
    void UpdateEffects(double delta_time) {

        effects_->Update(delta_time);
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
    EffectsManager * effects_;
    GameDefinitions::GameStateEnum state_;
};

#endif  // UNIVERSE_H_
