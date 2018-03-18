#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <Box2D.h>

#include <thread>

#include "game_data.h"

class Universe {
public:
    Universe() {}
    ~Universe() {
        thread_.join();
    }
    void Init() {
        game_data_ = &GAMEDATA;
        player_.x = 0.0;
        player_.y = 0.0;
        player_.a = 10.0;
        game_data_->SetPlayer(player_);

        planet_.x = 0.0;
        planet_.y = -100.0;
        planet_.a = 0.0;
        planet_.r = 90.0;
        game_data_->SetPlanet(planet_);

        Box2DInit();
    }
    void Box2DInit() {
        b2Vec2 gravity(0.0f, 0.0f);
        world_ = new b2World(gravity);

        // Planet creation
        b2_planet_def_.type = b2_kinematicBody;
        b2_planet_def_.position.Set(planet_.x, planet_.y);
        b2_planet_body_ = world_->CreateBody(&b2_planet_def_);

        b2CircleShape planet_disc;
        planet_disc.m_p.Set(0.0, 0.0);
        planet_disc.m_radius = planet_.r;

        b2FixtureDef b2_fixture;
        b2_fixture.density = 1.0;
        b2_fixture.friction = 0.7;

        b2_fixture.shape = &planet_disc;
        b2_planet_body_->CreateFixture(&b2_fixture);

        // Player creation
        b2_player_def_.type = b2_dynamicBody;
        b2_player_def_.angularVelocity = 0.0;
        b2_player_def_.angle = 10.0 * M_PI / 180.0;
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
        b2_fixture.density = 1.0;
        b2_fixture.friction = 0.7;

        b2_player_body_->CreateFixture(&b2_fixture);
    }
    void Run() {

        thread_ = std::thread(&Universe::ThreadLoop, this);
    }

private:
    void ThreadLoop() {
        while (true) {
            // Player
            b2Vec2 plr_pos = b2_player_body_->GetPosition();
            player_.x = plr_pos.x;
            player_.y = plr_pos.y;
            player_.a = b2_player_body_->GetAngle() * 180.0 / M_PI;
            game_data_->SetPlayer(player_);

            // Planet
            b2Vec2 plt_pos = b2_planet_body_->GetPosition();
            planet_.a = b2_planet_body_->GetAngle() * 180.0 / M_PI;
            game_data_->SetPlanet(planet_);

            // Gravity
            double dx = plt_pos.x - plr_pos.x;
            double dy = plt_pos.y - plr_pos.y;
            double r2 = dx*dx + dy*dy;
            double r = sqrt(r2);
            // planet mass
            double M = M_PI * planet_.r * planet_.r * 1.0;
            // player mass
            double m = 1.0;
            const double G = 1.0;
            double gmr2 = G * M * m / (r2 * r);
            double fx = gmr2 * dx;
            double fy = gmr2 * dy;
            b2Vec2 gravity(fx, fy);
            b2_player_body_->ApplyForceToCenter(gravity, true);

            // Planet rotation
            b2_planet_body_->SetAngularVelocity(0.2 * M_PI / 180.0);

            // Advance physics
            world_->Step(0.02, 6, 2);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

private:
    b2World * world_;
    b2BodyDef b2_player_def_;
    b2BodyDef b2_planet_def_;
    b2Body * b2_player_body_;
    b2Body * b2_planet_body_;

    std::thread thread_;
    GameData * game_data_;
    GameData::Player player_;
    GameData::Planet planet_;
};

#endif  // UNIVERSE_H_
