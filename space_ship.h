#ifndef SPACE_SHIP_H_
#define SPACE_SHIP_H_

#include <Box2D.h>
#include <GLFW/glfw3.h>

#include <functional>

#include "data_bus.h"

#include "systems/engine_system_interface.h"
#include "systems/radar_system_interface.h"
#include "systems/ship_systems_manager.h"

#include "devices/generic_hud_device.h"
#include "devices/hotas_device.h"

#define NUM_SHIP_VERTICES 8

class SpaceShip {
private:
    // Replaceable ship systems
    EngineSystemInterface * engine_;
    RadarSystemInterface * radar_;
    GenericHudDevice hud_;
    HOTASDevice hotas_;

    // TODO
    const float kMaxHullStrength;
    const float kImpulseThreshold;
    float hull_strength_;

private:
    double angle_;
    double mass_;
    double density_;
    double thrust_force_;
    double moment_;
    
    b2Vec2 position_;
    b2Vec2 velocity_;
    b2Vec2 gravity_;
    b2Vec2 thrust_;
    b2Body * physics_body_;
    b2Vec2 vertices_[NUM_SHIP_VERTICES];

    float color_[3];
public:
    SpaceShip() 
    : engine_(0)
    , radar_(0)
    , kMaxHullStrength(10.0)
    , kImpulseThreshold(1.0)
    , hull_strength_(kMaxHullStrength)
    , angle_(0.0)
    , mass_(1.0)
    , density_(1.0)
    , thrust_force_(0.0)
    , position_{ 0.0, 0.0 }
    , velocity_{ 0.0, 0.0 }
    , gravity_{ 0.0, 0.0 }
    , thrust_{ 0.0, 0.0 }
    , physics_body_(0)
    , vertices_{ { 0.16,  1.2 },
                 {-0.16,  1.2 },
                 {-0.5 ,  0.16},
                 {-0.68,  0.14},
                 {-0.54, -0.46},
                 { 0.54, -0.46},
                 { 0.68,  0.14},
                 { 0.5 ,  0.16}}
    , color_{ 1.0, 1.0, 1.0 }
    {
        engine_ = SYSTEMSMGR.getEngineSystem();
        radar_ = SYSTEMSMGR.getRadarSystem();

        using std::placeholders::_1;
        engine_->ThrustOutputHandler(std::bind(&SpaceShip::hndThrustOut, this, _1));
        engine_->MomentOutputHandler(std::bind(&SpaceShip::hndMomentOut, this, _1));
    }
    ~SpaceShip() {}
    void SetAngle(double angle) {
        angle_ = angle;
    }
    double GetAngle() {
        return angle_;
    }
    void SetPosition(double x, double y) {
        position_.x = x;
        position_.y = y;
    }
    b2Vec2 const & GetPosition() {
        return position_;
    }
    double GetSpeed() {
        return velocity_.Length();
    }
    double Mass() {
        return 1.4744 * density_;
    }
    void SetGravityAcceleration(const b2Vec2 & g) {
        gravity_ = Mass() * g;

        // Send player gravity to Data Bus.
        BD_Vector mg;
        mg.x = gravity_.x;
        mg.y = gravity_.y;
        DATABUS.Publish(db_PlayerGravity, &mg);
    }
    void ProcessImpulse(float impulse) {
        // TODO
        if (impulse > kImpulseThreshold) {
            hull_strength_ -= impulse;
            if (hull_strength_ <= 0.0) {
                hud_.Disable();
                hotas_.Disable();
            }
        }
    }
    void BeginContact() {
    }
    void EndContact() {
    }

    void Init(b2World * world) {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.angularVelocity = 0.0;
        def.angle = angle_ * M_PI / 180.0;
        def.position.Set(position_.x, position_.y);
        physics_body_ = world->CreateBody(&def);

        physics_body_->SetUserData(this);

        b2PolygonShape shape;
        shape.Set(vertices_, NUM_SHIP_VERTICES);

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = density_;
        fixture.friction = 0.7;

        physics_body_->CreateFixture(&fixture);

        // Init engine system.
        engine_->Init();
        radar_->Init();

        hud_.Init();
        hotas_.Init();
    }
    // Begin -- Handlers for Engine system.
    void hndThrustOut(double value) {
        thrust_force_ = value;

        BD_Scalar thrust;
        thrust.value = thrust_force_;
        DATABUS.Publish(db_PlayerThrust, &thrust);
    }
    void hndMomentOut(double value) {
        moment_ = value;
    }
    // End
    void Update(double delta_time) {
        // Gravity
        physics_body_->ApplyForceToCenter(gravity_, true);
        // Thrust
        thrust_.x = thrust_force_ * cos(0.5 * M_PI + physics_body_->GetAngle());
        thrust_.y = thrust_force_ * sin(0.5 * M_PI + physics_body_->GetAngle());
        physics_body_->ApplyForceToCenter(thrust_, true);
        // Moment
        physics_body_->ApplyTorque(moment_, true);
        // Get velocity for devices.
        velocity_ = physics_body_->GetLinearVelocity();

        // Get position.
        position_ = physics_body_->GetPosition();
        angle_ = physics_body_->GetAngle() * 180.0 / M_PI;

        // Send player velocity to Data Bus.
        BD_Vector v;
        v.x = velocity_.x;
        v.y = velocity_.y;
        DATABUS.Publish(db_PlayerVelocity, &v);

        BD_Vector p;
        p.x = position_.x;
        p.y = position_.y;
        DATABUS.Publish(db_PlayerPosition, &p);

        BD_Scalar s;
        s.value = angle_;
        DATABUS.Publish(db_PlayerAngle, &s);

        engine_->Update(delta_time);
        radar_->Update(delta_time);
    }
    void Render() {
        RenderShip();
        DISPLAY.UiMode();
        hud_.Render();
    }
    void RenderShip() {
        glLoadIdentity();
        glColor3fv(color_);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(0.0, 0.0);
        for (int i=0; i<NUM_SHIP_VERTICES; ++i)
        glVertex2d( vertices_[i].x, vertices_[i].y );
        glVertex2d( vertices_[0].x, vertices_[0].y );
        glEnd();
    }
    void HotasInput(int key, bool action) {
        switch(key) {
        case GLFW_KEY_W: // main thruster
            if (action == true) {
                // enable thruster.
                hotas_.SetThrottle(1.0);
            }
            else {
                // disable thruster.
                hotas_.SetThrottle(0.0);
            }
            break;
        case GLFW_KEY_A: // right thruster
            if (action == true) {
                hotas_.SetSteering(-1.0);
            }
            else {
                hotas_.SetSteering(0.0);
            }
            break;
        case GLFW_KEY_D: // left thruster
            if (action == true) {
                hotas_.SetSteering(1.0);
            }
            else {
                hotas_.SetSteering(0.0);
            }
            break;
        case GLFW_KEY_G:
            hotas_.ToggleLandingGear();
            break;
        }
    }
};

#endif // SPACE_SHIP_H_

