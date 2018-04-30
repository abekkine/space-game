#ifndef SPACE_SHIP_H_
#define SPACE_SHIP_H_

#include <Box2D.h>
#include <GLFW/glfw3.h>

#include <functional>

#include "effects_manager.h"
#include "solar_system.h"
#include "object_manager.h"

#include "data_bus.h"

#include "systems/engine_system_interface.h"
#include "systems/radar_system_interface.h"
#include "systems/hull_system_interface.h"
#include "systems/ship_systems_manager.h"
#include "systems/generic_hud_device.h"
#include "systems/hotas_device.h"

#include "contact_interface.h"

#include <assert.h>

#define NUM_PART_VERTICES 5

class SpaceShip : public ContactInterface {
private:
    DataBus * data_bus_;
    DataBus::Connection * bus_connection_;
    // BEGIN -- Ship Systems
    HudSystemInterface * hud_;
    HotasSystemInterface * hotas_;
    EngineSystemInterface * engine_;
    RadarSystemInterface * radar_;
    HullSystemInterface * hull_;
    // END -- Ship Systems
    EffectsManager * effects_;

    double angle_main_;
    double angle_upper_;
    double angle_left_;
    double angle_right_;

    double mass_;
    double density_;
    double thrust_force_;
    double moment_;

    double angular_velocity_;
    b2Vec2 pos_main_;
    b2Vec2 pos_upper_;
    b2Vec2 pos_left_;
    b2Vec2 pos_right_;

    b2Vec2 velocity_;
    b2Vec2 gravity_;
    b2Vec2 thrust_;
    b2Body * body_main_;
    b2Body * body_upper_;
    b2Body * body_left_;
    b2Body * body_right_;
    b2Vec2 v_main_body_[NUM_PART_VERTICES];
    b2Vec2 v_upper_body_[NUM_PART_VERTICES];
    b2Vec2 v_left_body_[NUM_PART_VERTICES];
    b2Vec2 v_right_body_[NUM_PART_VERTICES];

    b2Vec2 v_left_gear_[NUM_PART_VERTICES];
    b2Vec2 v_right_gear_[NUM_PART_VERTICES];

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

    , angle_main_(0.0)
    , angle_upper_(0.0)
    , angle_left_(0.0)
    , angle_right_(0.0)
    , mass_(1.0)
    , density_(1.0)
    , thrust_force_(0.0)
    , moment_(0.0)

    , angular_velocity_(0.0)
    , pos_main_{ 0.0, 0.0 }
    , pos_upper_{ 0.0, 0.0 }
    , pos_left_{ 0.0, 0.0 }
    , pos_right_{ 0.0, 0.0 }
    , velocity_{ 0.0, 0.0 }
    , gravity_{ 0.0, 0.0 }
    , thrust_{ 0.0, 0.0 }
    , body_main_(0)
    , body_upper_(0)
    , body_left_(0)
    , body_right_(0)
    , v_main_body_{{0.5, -0.24},
        {0.29, 0.41},
        {-0.29, 0.41},
        {-0.5, -0.24},
        {0.0, -0.43}}

    , v_upper_body_{{0.16, 0.26},
        {-0.16, 0.26},
        {-0.29, -0.13},
        {0.0, -0.28},
        {0.29, -0.13}}

    , v_left_body_{{-0.22, -0.3},
        {0.32, -0.3},
        {0.32, 0.16},
        {-0.14, 0.33},
        {-0.36, 0.3}}

    , v_right_body_{{-0.32, -0.3},
        {0.22, -0.3},
        {0.36, 0.3},
        {0.14, 0.33},
        {-0.32, 0.16}}

//    , v_left_gear_ { {-0.204, 0.26},
//                     {-0.064, -0.34},
//                     {0.046, -0.34},
//                     {0.226, 0.15},
//                     {-0.084, 0.27}}
//    , v_right_gear_ {{-0.046, -0.34},
//                     {0.064, -0.34},
//                     {0.204, 0.26},
//                     {0.084, 0.27},
//                     {-0.226, 0.15}}

    , color_{ 1.0, 1.0, 1.0 }
    , destroyed_(false)
    {
        data_bus_ = new DataBus();
        // TODO : Ideally, SpaceShip class should not have any direct connections to ship systems / devices (#115).
        bus_connection_ = data_bus_->Connect("ship");

        hud_ = SYSTEMSMGR.getHudSystem();
        hotas_ = SYSTEMSMGR.getHotasSystem();
        engine_ = SYSTEMSMGR.getEngineSystem();
        radar_ = SYSTEMSMGR.getRadarSystem();
        hull_ = SYSTEMSMGR.getHullSystem();

        hotas_->ConnectEngine(engine_);

        using std::placeholders::_1;
        engine_->ThrustOutputHandler(std::bind(&SpaceShip::hndThrustOut, this, _1));
        engine_->MomentOutputHandler(std::bind(&SpaceShip::hndMomentOut, this, _1));
        hull_->SetDestructionCallback(std::bind(&SpaceShip::OnDestroy, this));
    }
    ~SpaceShip() {}
    void SetAngle(double angle) {
        angle_main_ = angle;
    }
    double GetAngle() {
        return angle_main_;
    }
    void SetPosition(double x, double y) {
        pos_main_.x = x;
        pos_main_.y = y;
    }
    b2Vec2 const & GetPosition() {
        return pos_main_;
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
    void UpdateGravity() {

        SolarSystem* s = static_cast<SolarSystem *>(OBJMGR.Get("solar"));

        b2Vec2 g;
        b2Vec2 g_total;
        g = s->GetGravityAcceleration(pos_main_);
        body_main_->ApplyForceToCenter(0.6085 * g, true);
        g_total = (0.6085 + 0.219 + 0.688) * g;

        g = s->GetGravityAcceleration(pos_upper_);
        body_upper_->ApplyForceToCenter(0.219 * g, true);

        g = s->GetGravityAcceleration(pos_left_);
        body_left_->ApplyForceToCenter(0.344 * g, true);

        g = s->GetGravityAcceleration(pos_right_);
        body_right_->ApplyForceToCenter(0.344 * g, true);

        BD_Vector mg;
        mg.x = g_total.x;
        mg.y = g_total.y;
        if (bus_connection_ != 0) {
            bus_connection_->Publish(db_ShipGravity, &mg);
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
    void BeginContact(ContactInterface* object) {
        (void)object;
        // TODO : Use this to connect to object.
    }
    void EndContact(ContactInterface* object) {
        (void)object;
        // TODO : Use this to disconnect from object.
    }

    void Init(b2World * world) {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;

        b2FixtureDef fd;
        fd.density = density_;
        fd.friction = 0.7;
        b2PolygonShape shape;

        // Main Body
        bd.position.Set(pos_main_.x, pos_main_.y);
        body_main_ = world->CreateBody(&bd);
        body_main_->SetUserData(this);

        shape.Set(v_main_body_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_main_->CreateFixture(&fd);

        // Upper Body
        bd.position.Set(pos_main_.x, pos_main_.y + 0.5413);
        body_upper_ = world->CreateBody(&bd);
        body_upper_->SetUserData(this);

        shape.Set(v_upper_body_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_upper_->CreateFixture(&fd);

        // Left Body
        bd.position.Set(pos_main_.x - 0.3228, pos_main_.y - 0.5632);
        body_left_ = world->CreateBody(&bd);
        body_left_->SetUserData(this);

        shape.Set(v_left_body_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_left_->CreateFixture(&fd);

        // Right Body
        bd.position.Set(pos_main_.x + 0.3228, pos_main_.y - 0.5632);
        body_right_ = world->CreateBody(&bd);
        body_right_->SetUserData(this);

        shape.Set(v_right_body_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_right_->CreateFixture(&fd);

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
        UpdateGravity();
        // Thrust
        if (!destroyed_) {
            thrust_.x = thrust_force_ * cos(0.5 * M_PI + body_main_->GetAngle());
            thrust_.y = thrust_force_ * sin(0.5 * M_PI + body_main_->GetAngle());
            body_main_->ApplyForceToCenter(thrust_, true);

            // TODO : re-align thruster position.
            effects_->MainThruster(thrust_, pos_main_, velocity_);

            // Moment
            body_main_->ApplyTorque(moment_, true);

            // TODO : re-align thruster position.
            effects_->BowThruster(moment_, angle_main_, pos_main_, velocity_);
        }
        else {
            thrust_force_ = 0.0;
            moment_ = 0.0;
            thrust_.x = 0.0;
            thrust_.y = 0.0;
        }
        // Get angular velocity for devices
        angular_velocity_ = body_main_->GetAngularVelocity();
        // Get velocity for devices.
        velocity_ = body_main_->GetLinearVelocity();
        double speed = velocity_.Length();
        double lf = GameDefinitions::LorentzFactor(speed);
        body_main_->GetFixtureList()->SetDensity( density_ / lf );
        // Get position.
        pos_main_ = body_main_->GetPosition();
        angle_main_ = body_main_->GetAngle() * 180.0 / M_PI;

        angle_upper_ = body_upper_->GetAngle() * 180.0 / M_PI;
        angle_left_ = body_left_->GetAngle() * 180.0 / M_PI;
        angle_right_ = body_right_->GetAngle() * 180.0 / M_PI;

        pos_upper_ = body_upper_->GetPosition();
        pos_left_ = body_left_->GetPosition();
        pos_right_ = body_right_->GetPosition();

        if(bus_connection_ != 0) {
            // Send player velocity to Data Bus.
            // TODO : Should be published by navigation system (#115).
            // Used by HUD system.
            BD_Vector v;
            v.x = velocity_.x;
            v.y = velocity_.y;
            bus_connection_->Publish(db_ShipVelocity, &v);

            // TODO : Should be published by a sensor device (#115).
            BD_Scalar av;
            av.value = angular_velocity_;
            bus_connection_->Publish(db_ShipAngularVelocity, &av);

            // TODO : Should be published by a sensor device (#115).
            // Used by HUD & Radar systems.
            BD_Vector p;
            p.x = pos_main_.x;
            p.y = pos_main_.y;
            bus_connection_->Publish(db_ShipPosition, &p);

            // TODO : Should be published by a sensor device (#115).
            // Used by HUD system.
            BD_Scalar s;
            s.value = angle_main_;
            bus_connection_->Publish(db_ShipAngle, &s);

            // TODO : Thrust value should be published by engine system itself (#116).
            // TODO : LF should affect acceleration value, not thrust.
            // Used by HUD system.
            BD_Scalar thrust;
            thrust.value = thrust_force_ * lf;
            bus_connection_->Publish(db_ShipThrust, &thrust);
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

        int draw_mode = GL_LINE_LOOP;
        // Main Body
        glBegin(draw_mode);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_main_body_[i].x, v_main_body_[i].y);
        }
        glEnd();
        // Upper Body
        glPushMatrix();
        glLoadIdentity();
        glRotated(angle_main_, 0.0, 0.0, -1.0);
        glTranslated(pos_upper_.x - pos_main_.x, pos_upper_.y - pos_main_.y, 0.0);
        glRotated(angle_upper_, 0.0, 0.0, 1.0);
        glBegin(draw_mode);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_upper_body_[i].x, v_upper_body_[i].y);
        }
        glEnd();
        // Left Body
        glLoadIdentity();
        glRotated(angle_main_, 0.0, 0.0, -1.0);
        glTranslated(pos_left_.x - pos_main_.x, pos_left_.y - pos_main_.y, 0.0);
        glRotated(angle_left_, 0.0, 0.0, 1.0);
        glBegin(draw_mode);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_left_body_[i].x, v_left_body_[i].y);
        }
        glEnd();
        // Right Body
        glLoadIdentity();
        glRotated(angle_main_, 0.0, 0.0, -1.0);
        glTranslated(pos_right_.x - pos_main_.x, pos_right_.y - pos_main_.y, 0.0);
        glRotated(angle_right_, 0.0, 0.0, 1.0);
        glBegin(draw_mode);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_right_body_[i].x, v_right_body_[i].y);
        }
        glEnd();
        glPopMatrix();

//        glBegin(GL_TRIANGLE_FAN);
//        glVertex2d(0.0, 0.0);
//        for (int i=0; i<NUM_SHIP_VERTICES; ++i)
//        glVertex2d( vertices_[i].x, vertices_[i].y );
//        glVertex2d( vertices_[0].x, vertices_[0].y );
//        glEnd();
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

