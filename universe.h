#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <Box2D.h>

#include <thread>

#include "game_data.h"
#include "data_bus.h"
#include "systems/ship_systems_manager.h"

#include "planet.h"

class Universe {
public:
    Universe()
    : kGravityConstant(0.5)
    , kNumPlanets(3)
    {}
    ~Universe() {
        thread_.join();
    }
    void Init() {
        game_data_ = &GAMEDATA;
        player_.x = 0.0;
        player_.y = 0.0;
        player_.angle = 10.0;
        game_data_->SetPlayer(player_);

        planet_ = new Planet[kNumPlanets];

        planet_[0].SetPosition(0.0, -100.0);
        planet_[0].SetAngle(0.0);
        planet_[0].SetRadius(90.0);

        planet_[1].SetPosition(1000.0, 500.0);
        planet_[1].SetAngle(10.0);
        planet_[1].SetRadius(60.0);

        planet_[2].SetPosition(-500.0, -50.0);
        planet_[2].SetAngle(90.0);
        planet_[2].SetRadius(40.0);

        game_data_->SetPlanet(planet_);
        game_data_->SetNumPlanets(kNumPlanets);

        Box2DInit();

        t_begin_ = std::chrono::steady_clock::now();
        t_end_ = t_begin_;

        // TODO : check again, if this fits here.
        SYSTEMSMGR.getEngineSystem()->Init();
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
            planet_[i].Init(world_);
        }
    }
    void InitPlayer() {
        b2FixtureDef b2_fixture;

        b2_player_def_.type = b2_dynamicBody;
        b2_player_def_.angularVelocity = 0.0;
        b2_player_def_.angle = player_.angle * M_PI / 180.0;
        b2_player_def_.position.Set(player_.x, player_.y);
        b2_player_body_ = world_->CreateBody(&b2_player_def_);
        // player shape
        b2Vec2 v[8];
        for (int i=0; i<8; ++i) {
            v[i].Set(player_.vertices[i][0], player_.vertices[i][1]);
        }
        b2PolygonShape player_shape;
        player_shape.Set(v, 8);

        b2_fixture.shape = &player_shape;
        b2_fixture.density = player_.density;
        b2_fixture.friction = 0.7;

        b2_player_body_->CreateFixture(&b2_fixture);
    }
    void Run() {

        thread_ = std::thread(&Universe::ThreadLoop, this);
    }

private:
    void ThreadLoop() {
        while (true) {
            RefreshPositions();

            UpdateGravity();

            UpdatePlayer();

            UpdatePlanet();

            Step();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    void UpdateGravity() {
        // Gravity
        b2Vec2 g_factor;

        gravity_ = b2Vec2(0.0, 0.0);
        for (int i=0; i<kNumPlanets; ++i) {

            g_factor = planet_[i].GetGravityFactor(player_.x, player_.y);

            gravity_ += kGravityConstant * player_.Mass() * g_factor;
        }

        // Send player gravity to Data Bus.
        BD_Vector player_gravity;
        player_gravity.x = gravity_.x;
        player_gravity.y = gravity_.y;
        DATABUS.Publish(db_PlayerGravity, &player_gravity);
    }
    void UpdatePlayer() {
        // Gravity
        b2_player_body_->ApplyForceToCenter(gravity_, true);

        // Thrust
        double main_thrust_x;
        double main_thrust_y;
        game_data_->GetThrust(main_thrust_x, main_thrust_y);
        b2_player_body_->ApplyForceToCenter(b2Vec2(main_thrust_x, main_thrust_y), true);
        // Moment
        double moment = game_data_->GetMoment();
        b2_player_body_->ApplyTorque(moment, true);

        // Get Speed of player
        player_velocity_ = b2_player_body_->GetLinearVelocity();

        // Send player velocity to Data Bus.
        BD_Vector player_velocity;
        player_velocity.x = player_velocity_.x;
        player_velocity.y = player_velocity_.y;
        DATABUS.Publish(db_PlayerVelocity, &player_velocity);
    }
    void UpdatePlanet() {

        for (int i=0; i<kNumPlanets; ++i) {
            planet_[i].Update();
        }
    }
    void Step() {
        // Advance physics
        t_begin_ = std::chrono::steady_clock::now();
        double delta_time = std::chrono::duration<double> (t_begin_ - t_end_).count();
        if (game_data_->GetState() != GameDefinitions::gameState_InGame) {
            world_->Step(0.0, 12, 6);
        }
        else {
            world_->Step(delta_time, 12, 6);
        }

        // TODO : check again, if this fits here
        SYSTEMSMGR.getEngineSystem()->Update(delta_time);

        t_end_ = t_begin_;
    }
    void RefreshPositions() {
        // Player
        b2Vec2 pos;
        pos = b2_player_body_->GetPosition();
        player_.x = pos.x;
        player_.y = pos.y;
        player_.angle = b2_player_body_->GetAngle() * 180.0 / M_PI;
        player_.speed = player_velocity_.Length();
        // Send it up
        game_data_->SetPlayer(player_);

        // Send player gravity to Data Bus.
        BD_BasicPosition player_pos;
        player_pos.x = player_.x;
        player_pos.y = player_.y;
        player_pos.angle = player_.angle;
        DATABUS.Publish(db_PlayerPosition, &player_pos);
    }

private:
    const double kGravityConstant;
    const int kNumPlanets;
    b2Vec2 gravity_;
    std::chrono::time_point<std::chrono::steady_clock> t_begin_;
    std::chrono::time_point<std::chrono::steady_clock> t_end_;
    b2World * world_;
    b2BodyDef b2_player_def_;
    b2Body * b2_player_body_;
    b2Vec2 player_velocity_;

    std::thread thread_;
    GameData * game_data_;
    GameData::Player player_;
    Planet * planet_;
};

#endif  // UNIVERSE_H_
