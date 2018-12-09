#ifndef SPACE_SHIP_H_
#define SPACE_SHIP_H_

#include <Box2D.h>
#include <GLFW/glfw3.h>

#include <functional>

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
#include "station_interface.h"

#include <assert.h>

#define NUM_SHIP_VERTICES 8
#define NUM_PART_VERTICES 5

#define ENABLE_LANDING_GEAR

class SpaceShip : public ContactInterface {
private:
    // Ship communication bus
    DataBus * data_bus_;
    // Ship communication bus connection
    DataBus::Connection * bus_connection_;
    // Ship Systems
    // -- HUD system
    HudSystemInterface * hud_;
    // -- HOTAS system
    HotasSystemInterface * hotas_;
    // -- Engine system
    EngineSystemInterface * engine_;
    // -- Radar system
    RadarSystemInterface * radar_;
    // -- Hull system
    HullSystemInterface * hull_;

    // Body rotation angles
    double angle_;
    // -- left landing gear body angle
    double angle_llg_;
    // -- right landing gear body angle
    double angle_rlg_;

    // TODO : Ship mass; not used anywhere, yet.
    double mass_;
    // TODO : Ship material density (not used with multiple bodies)
    double density_;

    // Ship angular velocity.
    double angular_velocity_;

    // Body positions
    // -- main body position
    b2Vec2 position_;
    // -- left landing gear position
    b2Vec2 pos_llg_;
    // -- right landing gear position
    b2Vec2 pos_rlg_;

    // Velocity vector for ship.
    b2Vec2 velocity_;
    // Box2D world reference.
    b2World * world_;
    // Main part of ship body; camera fixed on this.
    b2Body * physics_body_;
    // Left Landing Gear itself.
    b2Body * body_left_gear_;
    // Right Landing Gear itself.
    b2Body * body_right_gear_;

    // Vertices for main ship body.
    b2Vec2 vertices_[NUM_SHIP_VERTICES];
    // Vertices for left landing gear.
    b2Vec2 v_left_gear_[NUM_PART_VERTICES];
    // Vertices for right landing gear.
    b2Vec2 v_right_gear_[NUM_PART_VERTICES];
    // Suspension joint for left landing gear.
    b2PrismaticJoint *j_llg_;
    // Suspension joint for right landing gear.
    b2PrismaticJoint *j_rlg_;

    // Landing gear up/down control variable.
    uint8_t landing_gear_state_;
    StationInterface* station_iface_;
    bool ship_anchored_;
    b2Joint *anchor_;
    // Ship color.
    float color_[3];
    // Ship destruction flag.
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

    , angle_(0)
    , angle_llg_(0)
    , angle_rlg_(0)
    , mass_(1.0)
    , density_(1.0)

    , angular_velocity_(0.0)
    , position_{ 0.0, 0.0 }
    , pos_llg_{ 0.0, 0.0 }
    , pos_rlg_{ 0.0, 0.0 }
    , velocity_{ 0.0, 0.0 }
    , world_(0)
    , physics_body_(0)
    , body_left_gear_(0)
    , body_right_gear_(0)
    , vertices_{ { 0.16,  0.8 },
                 {-0.16,  0.8 },
                 {-0.5 , -0.24},
                 {-0.68, -0.26},
                 {-0.54, -0.86},
                 { 0.54, -0.86},
                 { 0.68, -0.26},
                 { 0.5 , -0.24}}

    , v_left_gear_ { {-0.204, 0.26},
                     {-0.064, -0.28},
                     {0.046, -0.28},
                     {0.226, 0.15},
                     {-0.084, 0.27}}
    , v_right_gear_ {{-0.046, -0.28},
                     {0.064, -0.28},
                     {0.204, 0.26},
                     {0.084, 0.27},
                     {-0.226, 0.15}}
    , j_llg_(0)
    , j_rlg_(0)

    , landing_gear_state_(0)
    , station_iface_(0)
    , ship_anchored_(false)
    , anchor_(0)
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
        hull_->SetDestructionCallback(std::bind(&SpaceShip::OnDestroy, this));
    }
    ~SpaceShip() {
        SYSTEMSMGR.Destroy();
        delete data_bus_;
    }
    char Type() { return 's'; }
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
    void UpdateGravity() {

        SolarSystem* s = static_cast<SolarSystem *>(OBJMGR.Get("solar"));
        if (s == 0) return;

        b2Vec2 g;
        g = s->GetGravityAcceleration(position_);
        physics_body_->ApplyForceToCenter(g, true);

        BD_Vector mg;
        mg.x = g.x;
        mg.y = g.y;
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
        if (object->Type() != 'S') return;

        StationInterface *s = static_cast<StationInterface *>(object);

        if (s != 0 && ship_anchored_ == false) {
            station_iface_ = s;
        }
    }
    void EndContact(ContactInterface* object) {
        StationInterface *s = static_cast<StationInterface *>(object);

        if (s != 0 && ship_anchored_ == false) {
            station_iface_ = 0;
        }
    }

    void Init(b2World * world) {
        world_ = world;

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.angularVelocity = 0.0;
        bd.angle = angle_ * M_PI / 180.0;
        bd.position.Set(position_.x, position_.y);
        physics_body_ = world_->CreateBody(&bd);
        physics_body_->SetUserData(this);

        b2PolygonShape shape;
        shape.Set(vertices_, NUM_SHIP_VERTICES);

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = density_;
        fd.friction = 0.7;

        physics_body_->CreateFixture(&fd);

#ifdef ENABLE_LANDING_GEAR
        // Landing gear bodies
        bd.position.Set(position_.x - 0.4361, position_.y - 0.56);
        body_left_gear_ = world_->CreateBody(&bd);
        body_left_gear_->SetUserData(this);
        shape.Set(v_left_gear_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_left_gear_->CreateFixture(&fd);

        bd.position.Set(position_.x + 0.4361, position_.y - 0.56);
        body_right_gear_ = world_->CreateBody(&bd);
        body_right_gear_->SetUserData(this);
        shape.Set(v_right_gear_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_right_gear_->CreateFixture(&fd);
        // End -- Landing gears.

        b2Vec2 anchor(position_.x, position_.y);

        // Begin -- Landing Gears
        const float maxForce = 10.0f;
        const float speed = 0.5f;
        const float lowerLim = -0.2f;
        const float upperLim = 0.01f;
        b2Vec2 axis(-1.25, 6.25);
        axis.Normalize();
        anchor.x = position_.x - 0.42;
        anchor.y = position_.y - 0.54;

        b2PrismaticJointDef pjdL;
        b2PrismaticJointDef pjdR;
        pjdL.Initialize(physics_body_, body_left_gear_, anchor, axis);
        axis.x = -axis.x;
        anchor.x = -anchor.x;
        pjdR.Initialize(physics_body_, body_right_gear_, anchor, axis);

        pjdL.lowerTranslation = lowerLim;
        pjdR.lowerTranslation = lowerLim;
        pjdL.upperTranslation = upperLim;
        pjdR.upperTranslation = upperLim;
        pjdL.enableLimit = true;
        pjdR.enableLimit = true;
        pjdL.motorSpeed = speed;
        pjdR.motorSpeed = speed;
        pjdL.maxMotorForce = maxForce;
        pjdR.maxMotorForce = maxForce;
        pjdL.enableMotor = true;
        pjdR.enableMotor = true;
        j_llg_ = static_cast<b2PrismaticJoint*>(world_->CreateJoint(&pjdL));
        j_rlg_ = static_cast<b2PrismaticJoint*>(world_->CreateJoint(&pjdR));
        // End -- Landing Gears
#endif // ENABLE_LANDING_GEAR

        // Init engine system.
        engine_->Init(data_bus_);
        engine_->Mount(physics_body_);

        radar_->Init(data_bus_);

        hud_->Init(data_bus_);
        hotas_->Init(data_bus_);
        hull_->Init(data_bus_);
    }
    // TODO : Box2D cannot handle multiple rotations (>360.0).
    void NormalizeAngle(b2Body* b) {
        double a = b->GetAngle();
        a = fmod(a, 2.0 * M_PI);
        b->SetTransform(b->GetPosition(), a);
    }
    void NormalizeAngles() {

        NormalizeAngle(physics_body_);
        NormalizeAngle(body_left_gear_);
        NormalizeAngle(body_right_gear_);
    }
    void Update(double delta_time) {
        // Gravity
        UpdateGravity();

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

#ifdef ENABLE_LANDING_GEAR
        angle_llg_ = body_left_gear_->GetAngle() * 180.0 / M_PI;
        angle_rlg_ = body_right_gear_->GetAngle() * 180.0 / M_PI;

        pos_llg_ = body_left_gear_->GetPosition();
        pos_rlg_ = body_right_gear_->GetPosition();
#endif

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
            p.x = position_.x;
            p.y = position_.y;
            bus_connection_->Publish(db_ShipPosition, &p);

            // TODO : Should be published by a sensor device (#115).
            // Used by HUD system.
            BD_Scalar s;
            s.value = angle_;
            bus_connection_->Publish(db_ShipAngle, &s);
        }

        engine_->Update(delta_time);
        radar_->Update(delta_time);
    }
    void Render() {
        RenderShip();
        DISPLAY.UiMode();
        hud_->Render();
        if (station_iface_ != 0 && ship_anchored_) {
            station_iface_->RenderUi();
        }
    }
    void RenderShip() {
        glLoadIdentity();
        glColor3fv(color_);

        // Main Body
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(0.0, 0.0);
        for (int i=0; i<NUM_SHIP_VERTICES; ++i) {
            glVertex2d(vertices_[i].x, vertices_[i].y);
        }
        glVertex2d(vertices_[0].x, vertices_[0].y);
        glEnd();

        glPushMatrix();

#ifdef ENABLE_LANDING_GEAR
        // Landing Gears
        glLoadIdentity();
        glRotated(angle_, 0.0, 0.0, -1.0);
        glTranslated(pos_llg_.x - position_.x, pos_llg_.y - position_.y, 0.0);
        glRotated(angle_llg_, 0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_left_gear_[i].x, v_left_gear_[i].y);
        }
        glEnd();
        glLoadIdentity();
        glRotated(angle_, 0.0, 0.0, -1.0);
        glTranslated(pos_rlg_.x - position_.x, pos_rlg_.y - position_.y, 0.0);
        glRotated(angle_rlg_, 0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_right_gear_[i].x, v_right_gear_[i].y);
        }
        glEnd();
 #endif // ENABLE_LANDING_GEAR
        glPopMatrix();
    }

    void AnchorShip() {
        if (station_iface_ != 0) {
            NormalizeAngles();
 #ifdef ENABLE_LANDING_GEAR
            anchor_ = station_iface_->AnchorShip(body_left_gear_);
            if (anchor_ != 0) {
                ship_anchored_ = true;
                station_iface_->Connect();
            }
 #endif
        }
    }

    void ReleaseShip() {
        assert(station_iface_ != 0 && "No station interface!");
        station_iface_->Disconnect();
 #ifdef ENABLE_LANDING_GEAR
        world_->DestroyJoint( anchor_ );
 #endif
        ship_anchored_ = false;
    }

    void RefuelRequest() {
        if (station_iface_ != 0 && ship_anchored_) {
            station_iface_->Refuel(engine_);
        }
    }

    void RepairRequest() {
        if (station_iface_ != 0 && ship_anchored_) {
            station_iface_->Repair(hull_);
        }
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
            break;
        case GLFW_KEY_F:
            if (action == true) {
                RefuelRequest();
            }
            break;
        case GLFW_KEY_R:
            if (action == true) {
                RepairRequest();
            }
            break;
        case GLFW_KEY_H:
            if (action == true) {
                if (ship_anchored_ == false) {
                    // Anchor ship to station
                    AnchorShip();
                } else {
                    // Release anchor
                    ReleaseShip();
                }
            }
            break;
        case GLFW_KEY_G:
            // TODO : pass landing gear command
            //      : to landing gear 'system' through hotas.
            // hotas_->ToggleLandingGear();
#ifdef ENABLE_LANDING_GEAR
            if (action) {
                if (landing_gear_state_) {
                    puts("LG Up!");
                    j_llg_->SetMotorSpeed(0.3);
                    j_rlg_->SetMotorSpeed(0.3);
                    landing_gear_state_ = 0;
                } else {
                    puts("LG Down!");
                    j_llg_->SetMotorSpeed(-0.3);
                    j_rlg_->SetMotorSpeed(-0.3);
                    landing_gear_state_ = 1;
                }
            }
#endif
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
