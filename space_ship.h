#ifndef SPACE_SHIP_H_
#define SPACE_SHIP_H_

#include <Box2D.h>
#include <GLFW/glfw3.h>

#include <functional>

#include "effects_manager.h"
#include "object_manager.h"

#include "data_bus.h"

#include "systems/engine_system_interface.h"
#include "systems/radar_system_interface.h"
#include "systems/hull_system_interface.h"
#include "systems/ship_systems_manager.h"

// TODO : Devices should be converted to systems.
#include "devices/generic_hud_device.h"
#include "devices/hotas_device.h"

#include <assert.h>

#define NUM_SHIP_VERTICES 8

class SpaceShip {
private:
    DataBus * data_bus_;
    DataBus::Connection * bus_connection_;
    // Replaceable ship systems
    GenericHudDevice * hud_;
    HOTASDevice * hotas_;
    EngineSystemInterface * engine_;
    RadarSystemInterface * radar_;
    HullSystemInterface * hull_;
    EffectsManager * effects_;

    double angle_;
    double mass_;
    double density_;
    double thrust_force_;
    double moment_;

    double angular_velocity_;
    b2Vec2 position_;
    b2Vec2 velocity_;
    b2Vec2 gravity_;
    b2Vec2 thrust_;
    b2Body * physics_body_;
    b2Vec2 vertices_[NUM_SHIP_VERTICES];

    float color_[3];

    bool destroyed_;
public:
    SpaceShip()
    : data_bus_(0)
    , bus_connection_(0)
    , hud_(0)
    , hotas_(0)
    , engine_(0)
    , radar_(0)
    , hull_(0)
    , effects_(0)

    , angle_(0.0)
    , mass_(1.0)
    , density_(1.0)
    , thrust_force_(0.0)
    , moment_(0.0)

    , angular_velocity_(0.0)
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
    , destroyed_(false)
    {
        data_bus_ = new DataBus();
        // TODO : Ideally, SpaceShip class should not have any direct connections to ship systems / devices.
        bus_connection_ = data_bus_->Connect("ship");

        hud_ = new GenericHudDevice();
        hotas_ = new HOTASDevice();

        engine_ = SYSTEMSMGR.getEngineSystem();
        radar_ = SYSTEMSMGR.getRadarSystem();
        hull_ = SYSTEMSMGR.getHullSystem();

        using std::placeholders::_1;
        engine_->ThrustOutputHandler(std::bind(&SpaceShip::hndThrustOut, this, _1));
        engine_->MomentOutputHandler(std::bind(&SpaceShip::hndMomentOut, this, _1));
        hull_->SetDestructionCallback(std::bind(&SpaceShip::OnDestroy, this));
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
        double volume = 0.0;
        double hull_volume = 1.4744;
        double fuel_volume = 0.0;

        if (engine_ != 0) {
            fuel_volume = engine_->FuelVolume();
        }
        volume = fuel_volume + hull_volume;

        return volume * density_;
    }
    void SetGravityAcceleration(const b2Vec2 & g) {
        gravity_ = Mass() * g;

        // Send player gravity to Data Bus.
        // TODO : This value should be published by a sensor device/system.
        BD_Vector mg;
        mg.x = gravity_.x;
        mg.y = gravity_.y;
        if (bus_connection_ != 0) {
            bus_connection_->Publish(db_PlayerGravity, &mg);
        }
    }
    void OnDestroy() {
        destroyed_ = true;
        hud_->Disable();
        hotas_->Disable();
    }
    void ProcessImpulse(float impulse) {

        hull_->ApplyImpact(impulse);
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
        engine_->Init(data_bus_);
        radar_->Init(data_bus_);

        hud_->Init(data_bus_);
        hotas_->Init(data_bus_);
        hull_->Init(data_bus_);

        effects_ = static_cast<EffectsManager*>(OBJMGR.Get("effects"));
    }
    // Begin -- Handlers for Engine system.
    void hndThrustOut(double value) {
        thrust_force_ = value;
    }
    void hndMomentOut(double value) {
        moment_ = value;
    }
    // End
    void Update(double delta_time) {
        // Gravity
        physics_body_->ApplyForceToCenter(gravity_, true);
        // Thrust
        if (!destroyed_) {
            thrust_.x = thrust_force_ * cos(0.5 * M_PI + physics_body_->GetAngle());
            thrust_.y = thrust_force_ * sin(0.5 * M_PI + physics_body_->GetAngle());
            physics_body_->ApplyForceToCenter(thrust_, true);

            effects_->MainThruster(thrust_, position_, velocity_);

            // Moment
            physics_body_->ApplyTorque(moment_, true);

            effects_->BowThruster(moment_, angle_, position_, velocity_);
        }
        else {
            thrust_force_ = 0.0;
            moment_ = 0.0;
            thrust_.x = 0.0;
            thrust_.y = 0.0;
        }
        // Get angular velocity for devices
        angular_velocity_ = physics_body_->GetAngularVelocity();
        // Get velocity for devices.
        velocity_ = physics_body_->GetLinearVelocity();
        double speed = velocity_.Length();
        double lf = GameDefinitions::LorentzFactor(speed);
        physics_body_->GetFixtureList()->SetDensity( density_ / lf );
        // Get position.
        position_ = physics_body_->GetPosition();
        angle_ = physics_body_->GetAngle() * 180.0 / M_PI;

        if(bus_connection_ != 0) {
            // Send player velocity to Data Bus.
            // TODO : Should be published by a sensor device.
            // Used by HUD system.
            BD_Vector v;
            v.x = velocity_.x;
            v.y = velocity_.y;
            bus_connection_->Publish(db_PlayerVelocity, &v);

            // TODO : Should be published by a sensor device.
            BD_Scalar av;
            av.value = angular_velocity_;
            bus_connection_->Publish(db_ShipAngularVelocity, &av);

            // TODO : Should be published by a sensor device.
            // Used by HUD & Radar systems.
            BD_Vector p;
            p.x = position_.x;
            p.y = position_.y;
            bus_connection_->Publish(db_PlayerPosition, &p);

            // TODO : Should be published by a sensor device.
            // Used by HUD system.
            BD_Scalar s;
            s.value = angle_;
            bus_connection_->Publish(db_PlayerAngle, &s);

            // TODO : Thrust value should be published by engine system itself.
            // TODO : LF should affect acceleration value, not thrust.
            // Used by HUD system.
            BD_Scalar thrust;
            thrust.value = thrust_force_ * lf;
            bus_connection_->Publish(db_PlayerThrust, &thrust);
        }

        engine_->Update(delta_time);
        radar_->Update(delta_time);
    }
    void Render() {
        RenderShip();
        DISPLAY.UiMode();
        hud_->Render();
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
        assert(hotas_ != 0);
        switch(key) {
        case GLFW_KEY_W: // main thruster
            if (action == true) {
                // enable thruster.
                hotas_->SetThrottle(1.0);
            }
            else {
                // disable thruster.
                hotas_->SetThrottle(0.0);
            }
            break;
        case GLFW_KEY_A: // right thruster
            if (action == true) {
                hotas_->SetSteering(-1.0);
            }
            else {
                hotas_->SetSteering(0.0);
            }
            break;
        case GLFW_KEY_D: // left thruster
            if (action == true) {
                hotas_->SetSteering(1.0);
            }
            else {
                hotas_->SetSteering(0.0);
            }
            break;
        case GLFW_KEY_S: // stop rotation
            if (action == true) {
                hotas_->Stabilize();
            }
        case GLFW_KEY_G:
            hotas_->ToggleLandingGear();
            break;

#ifdef DISCONNECT_TEST
        case GLFW_KEY_1: // hud
            hud_->Disconnect();
            break;
        case GLFW_KEY_2: // ship
            data_bus_->Disconnect("ship", bus_connection_);
            break;
        case GLFW_KEY_3: // engine
            engine_->Disconnect();
            break;
        case GLFW_KEY_4: // hull
            hull_->Disconnect();
            break;
        case GLFW_KEY_5: // radar
            radar_->Disconnect();
            break;
#endif // DISCONNECT_TEST
        }
    }
};

#endif // SPACE_SHIP_H_

