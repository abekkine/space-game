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
#include "station_interface.h"

#include <assert.h>

#define NUM_PART_VERTICES 5

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
    // Effects
    EffectsManager * effects_;

    // Body rotation angles
    // -- main body angle
    double angle_main_;
    // -- upper body angle
    double angle_upper_;
    // -- left landing gear 'bay' body angle
    double angle_left_;
    // -- right landing gear 'bay' body angle
    double angle_right_;
    // -- left landing gear body angle
    double angle_llg_;
    // -- right landing gear body angle
    double angle_rlg_;

    // TODO : Ship mass; not used anywhere, yet.
    double mass_;
    // TODO : Ship material density (not used with multiple bodies)
    double density_;
    // Force value for thrust vector.
    double thrust_force_;
    // Torque value for ship rotation.
    double moment_;

    // Ship angular velocity.
    double angular_velocity_;

    // Body positions
    // -- main body position
    b2Vec2 pos_main_;
    // -- upper body position
    b2Vec2 pos_upper_;
    // -- left landing gear bay position
    b2Vec2 pos_left_;
    // -- right landing gear bay position
    b2Vec2 pos_right_;
    // -- left landing gear position
    b2Vec2 pos_llg_;
    // -- right landing gear position
    b2Vec2 pos_rlg_;

    // Velocity vector for ship.
    b2Vec2 velocity_;
    // Ship thrust vector.
    b2Vec2 thrust_;
    // Box2D world reference.
    b2World * world_;
    // Main part of ship body; camera fixed on this.
    b2Body * body_main_;
    // Upper part of ship body; ship systems (proposal) reside in this.
    b2Body * body_upper_;
    // Left landing gear bay; left landing gear resides here when retracted.
    b2Body * body_left_;
    // Right landing gear bay; right landing gear resides here when retracted.
    b2Body * body_right_;
    // Left Landing Gear itself.
    b2Body * body_left_gear_;
    // Right Landing Gear itself.
    b2Body * body_right_gear_;

    // Vertices for main ship body.
    b2Vec2 v_main_body_[NUM_PART_VERTICES];
    // Vertices for upper ship body.
    b2Vec2 v_upper_body_[NUM_PART_VERTICES];
    // Vertices for left landing gear bay.
    b2Vec2 v_left_body_[NUM_PART_VERTICES];
    // Vertices for right landing gear bay.
    b2Vec2 v_right_body_[NUM_PART_VERTICES];
    // Vertices for left landing gear.
    b2Vec2 v_left_gear_[NUM_PART_VERTICES];
    // Vertices for right landing gear.
    b2Vec2 v_right_gear_[NUM_PART_VERTICES];
    // Joint between main & upper body parts.
    b2WeldJoint *j_upper_;
    // Joint between main & left gear bay body parts.
    b2WeldJoint *j_left_;
    // Joint between main & right gear bay body parts.
    b2WeldJoint *j_right_;
    // Joint between left & right gear bay body parts.
    b2WeldJoint *j_lr_;
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
    , effects_(0)

    , angle_main_(0.0)
    , angle_upper_(0.0)
    , angle_left_(0.0)
    , angle_right_(0.0)
    , angle_llg_(0)
    , angle_rlg_(0)
    , mass_(1.0)
    , density_(1.0)
    , thrust_force_(0.0)
    , moment_(0.0)

    , angular_velocity_(0.0)
    , pos_main_{ 0.0, 0.0 }
    , pos_upper_{ 0.0, 0.0 }
    , pos_left_{ 0.0, 0.0 }
    , pos_right_{ 0.0, 0.0 }
    , pos_llg_{ 0.0, 0.0 }
    , pos_rlg_{ 0.0, 0.0 }
    , velocity_{ 0.0, 0.0 }
    , thrust_{ 0.0, 0.0 }
    , world_(0)
    , body_main_(0)
    , body_upper_(0)
    , body_left_(0)
    , body_right_(0)
    , body_left_gear_(0)
    , body_right_gear_(0)
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
        {0.33, -0.3},
        {0.33, 0.16},
        {-0.14, 0.33},
        {-0.36, 0.3}}

    , v_right_body_{{-0.33, -0.3},
        {0.22, -0.3},
        {0.36, 0.3},
        {0.14, 0.33},
        {-0.33, 0.16}}

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
    , j_upper_(0)
    , j_left_(0)
    , j_right_(0)
    , j_lr_(0)
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
        g_total = (0.6085 + 0.219 + 0.688 + 0.314) * g;

        g = s->GetGravityAcceleration(pos_upper_);
        body_upper_->ApplyForceToCenter(0.219 * g, true);

        g = s->GetGravityAcceleration(pos_left_);
        body_left_->ApplyForceToCenter(0.344 * g, true);

        g = s->GetGravityAcceleration(pos_right_);
        body_right_->ApplyForceToCenter(0.344 * g, true);

        g = s->GetGravityAcceleration(pos_llg_);
        body_left_gear_->ApplyForceToCenter(0.157 * g, true);
        g = s->GetGravityAcceleration(pos_rlg_);
        body_right_gear_->ApplyForceToCenter(0.157 * g, true);

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
        StationInterface *s = static_cast<StationInterface *>(object);

        if (s != 0) {
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

        b2FixtureDef fd;
        fd.density = density_;
        fd.friction = 0.7;
        b2PolygonShape shape;

        // Main Body
        bd.position.Set(pos_main_.x, pos_main_.y);
        body_main_ = world_->CreateBody(&bd);
        body_main_->SetUserData(this);

        shape.Set(v_main_body_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_main_->CreateFixture(&fd);

        // Upper Body
        bd.position.Set(pos_main_.x, pos_main_.y + 0.5413);
        body_upper_ = world_->CreateBody(&bd);
        body_upper_->SetUserData(this);

        shape.Set(v_upper_body_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_upper_->CreateFixture(&fd);

        // Left Body
        bd.position.Set(pos_main_.x - 0.3228, pos_main_.y - 0.5632);
        body_left_ = world_->CreateBody(&bd);
        body_left_->SetUserData(this);

        shape.Set(v_left_body_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_left_->CreateFixture(&fd);

        // Right Body
        bd.position.Set(pos_main_.x + 0.3228, pos_main_.y - 0.5632);
        body_right_ = world_->CreateBody(&bd);
        body_right_->SetUserData(this);

        shape.Set(v_right_body_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_right_->CreateFixture(&fd);

        // Landing gear bodies
        bd.position.Set(pos_main_.x - 0.4361, pos_main_.y - 0.56);
        body_left_gear_ = world_->CreateBody(&bd);
        body_left_gear_->SetUserData(this);
        shape.Set(v_left_gear_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_left_gear_->CreateFixture(&fd);

        bd.position.Set(pos_main_.x + 0.4361, pos_main_.y - 0.56);
        body_right_gear_ = world_->CreateBody(&bd);
        body_right_gear_->SetUserData(this);
        shape.Set(v_right_gear_, NUM_PART_VERTICES);
        fd.shape = &shape;
        body_right_gear_->CreateFixture(&fd);
        // End -- Landing gears.

        // Ship body joints
        b2WeldJointDef wjd;
        b2Vec2 anchor(pos_main_.x, pos_main_.y);
        // -- Main to upper
        wjd.bodyA = body_main_;
        wjd.bodyB = body_upper_;
        wjd.Initialize(body_main_, body_upper_, anchor);
        wjd.localAnchorA = body_main_->GetLocalPoint(anchor);
        wjd.localAnchorB = body_upper_->GetLocalPoint(anchor);
        wjd.collideConnected = false;
        j_upper_ = static_cast<b2WeldJoint*>(world_->CreateJoint(&wjd));
        // -- Main to left
        wjd.bodyA = body_main_;
        wjd.bodyB = body_left_;
        wjd.Initialize(body_main_, body_left_, anchor);
        wjd.localAnchorA = body_main_->GetLocalPoint(anchor);
        wjd.localAnchorB = body_left_->GetLocalPoint(anchor);
        wjd.collideConnected = false;
        j_left_ = static_cast<b2WeldJoint*>(world_->CreateJoint(&wjd));
        // -- Main to right
        wjd.bodyA = body_main_;
        wjd.bodyB = body_right_;
        wjd.Initialize(body_main_, body_right_, anchor);
        wjd.localAnchorA = body_main_->GetLocalPoint(anchor);
        wjd.localAnchorB = body_right_->GetLocalPoint(anchor);
        wjd.collideConnected = false;
        j_right_ = static_cast<b2WeldJoint*>(world_->CreateJoint(&wjd));
        // -- Left to Right
        wjd.bodyA = body_left_;
        wjd.bodyB = body_right_;
        wjd.Initialize(body_left_, body_right_, anchor);
        wjd.localAnchorA = body_left_->GetLocalPoint(anchor);
        wjd.localAnchorB = body_right_->GetLocalPoint(anchor);
        wjd.collideConnected = false;
        j_lr_ = static_cast<b2WeldJoint*>(world_->CreateJoint(&wjd));

        // Begin -- Landing Gears
        const float maxForce = 10.0f;
        const float speed = 0.5f;
        const float lowerLim = -0.2f;
        const float upperLim = 0.01f;
        b2Vec2 axis(-1.25, 6.25);
        axis.Normalize();
        anchor.x = pos_main_.x - 0.42;
        anchor.y = pos_main_.y - 0.54;

        b2PrismaticJointDef pjdL;
        b2PrismaticJointDef pjdR;
        pjdL.Initialize(body_left_, body_left_gear_, anchor, axis);
        axis.x = -axis.x;
        anchor.x = -anchor.x;
        pjdR.Initialize(body_right_, body_right_gear_, anchor, axis);

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
    void CheckJoints(double delta_time) {

        double f1, f2, f3, f4;
        const double kBreakForce = 500.0;
        if (j_upper_) {
            f1 = j_upper_->GetReactionForce(1.0 / delta_time).Length();
            if (f1 > kBreakForce) {
                world_->DestroyJoint(j_upper_);
                j_upper_ = 0;
                puts("..Upper joint breaks!");
            }
        }

        if (j_left_) {
            f2 = j_left_->GetReactionForce(1.0 / delta_time).Length();
            if (f2 > kBreakForce) {
                world_->DestroyJoint(j_left_);
                j_left_ = 0;
                puts("..Left joint breaks!");
            }
        }

        if (j_right_) {
            f3 = j_right_->GetReactionForce(1.0 / delta_time).Length();
            if (f3 > kBreakForce) {
                world_->DestroyJoint(j_right_);
                j_right_ = 0;
                puts("..Right joint breaks!");
           }
        }

        if (j_lr_) {
            f4 = j_lr_->GetReactionForce(1.0 / delta_time).Length();
            if (f4 > kBreakForce) {
                world_->DestroyJoint(j_lr_);
                j_lr_ = 0;
                puts("..Bottom joint (LR) breaks!");
            }
        }
    }
    void Update(double delta_time) {
        // Check Joints
        CheckJoints(delta_time);
        // Gravity
        UpdateGravity();

        if (!destroyed_) {
            // Thrust
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
        angle_llg_ = body_left_gear_->GetAngle() * 180.0 / M_PI;
        angle_rlg_ = body_right_gear_->GetAngle() * 180.0 / M_PI;

        pos_upper_ = body_upper_->GetPosition();
        pos_left_ = body_left_->GetPosition();
        pos_right_ = body_right_->GetPosition();
        pos_llg_ = body_left_gear_->GetPosition();
        pos_rlg_ = body_right_gear_->GetPosition();

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

        // Main Body
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(0.0, 0.0);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_main_body_[i].x, v_main_body_[i].y);
        }
        glVertex2d(v_main_body_[0].x, v_main_body_[0].y);
        glEnd();
        // Upper Body
        glPushMatrix();
        glLoadIdentity();
        glRotated(angle_main_, 0.0, 0.0, -1.0);
        glTranslated(pos_upper_.x - pos_main_.x, pos_upper_.y - pos_main_.y, 0.0);
        glRotated(angle_upper_, 0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_upper_body_[i].x, v_upper_body_[i].y);
        }
        glEnd();
        // Left Body
        glLoadIdentity();
        glRotated(angle_main_, 0.0, 0.0, -1.0);
        glTranslated(pos_left_.x - pos_main_.x, pos_left_.y - pos_main_.y, 0.0);
        glRotated(angle_left_, 0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_left_body_[i].x, v_left_body_[i].y);
        }
        glEnd();
        // Right Body
        glLoadIdentity();
        glRotated(angle_main_, 0.0, 0.0, -1.0);
        glTranslated(pos_right_.x - pos_main_.x, pos_right_.y - pos_main_.y, 0.0);
        glRotated(angle_right_, 0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_right_body_[i].x, v_right_body_[i].y);
        }
        glEnd();

        // Landing Gears
        glLoadIdentity();
        glRotated(angle_main_, 0.0, 0.0, -1.0);
        glTranslated(pos_llg_.x - pos_main_.x, pos_llg_.y - pos_main_.y, 0.0);
        glRotated(angle_llg_, 0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_left_gear_[i].x, v_left_gear_[i].y);
        }
        glEnd();
        glLoadIdentity();
        glRotated(angle_main_, 0.0, 0.0, -1.0);
        glTranslated(pos_rlg_.x - pos_main_.x, pos_rlg_.y - pos_main_.y, 0.0);
        glRotated(angle_rlg_, 0.0, 0.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (int i=0; i<NUM_PART_VERTICES; ++i) {
            glVertex2d(v_right_gear_[i].x, v_right_gear_[i].y);
        }
        glEnd();
        glPopMatrix();
    }

    void AnchorShip() {
        if (station_iface_ != 0) {
            anchor_ = station_iface_->AnchorShip(body_left_gear_);
            if (anchor_ != 0) {
                ship_anchored_ = true;
                station_iface_->Connect();
            }
        }
    }

    void ReleaseShip() {
        station_iface_->Disconnect();
        world_->DestroyJoint( anchor_ );
        ship_anchored_ = false;
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
