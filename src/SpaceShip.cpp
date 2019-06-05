#include "SpaceShip.h"
#include "GameDefinitions.h"
#include "DataBus.h"
#include "DataBusConnection.h"
#include "HotasSystemInterface.h"
#include "EngineSystemInterface.h"
#include "HudSystemInterface.h"
#include "RadarSystemInterface.h"
#include "HullSystemInterface.h"
#include "ShipSystemsManager.h"
#include "StationInterface.h"
#include "SolarSystemInterface.h"
#include "ObjectManager.h"
#include "Display.h"

#include <GLFW/glfw3.h>

#include <assert.h>

SpaceShip::SpaceShip()
: data_bus_(0)
, bus_connection_(0)
, hud_(0)
, hotas_(0)
, engine_(0)
, radar_(0)
, hull_(0)

, angle_main_(0.0)
, angle_upper_(0.0)
, angle_left_(0.0)
, angle_right_(0.0)
, angle_llg_(0)
, angle_rlg_(0)
, mass_(1.0)
, density_(1.0)

, angular_velocity_(0.0)
, pos_main_{ 0.0, 0.0 }
, pos_upper_{ 0.0, 0.0 }
, pos_left_{ 0.0, 0.0 }
, pos_right_{ 0.0, 0.0 }
, pos_llg_{ 0.0, 0.0 }
, pos_rlg_{ 0.0, 0.0 }
, velocity_{ 0.0, 0.0 }
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
    // NOTE : Ideally, SpaceShip class should not have any direct connections to ship systems / devices (#115).
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

SpaceShip::~SpaceShip() {
    SYSTEMSMGR.Destroy();
    delete data_bus_;
}

char SpaceShip::Type() {
    return 's';
}

void SpaceShip::SetAngle(double angle) {
    angle_main_ = angle;
}

double SpaceShip::GetAngle() {
    return angle_main_;
}

void SpaceShip::SetPosition(double x, double y) {
    pos_main_.x = x;
    pos_main_.y = y;
}

b2Vec2 const & SpaceShip::GetPosition() {
    return pos_main_;
}

double SpaceShip::GetSpeed() {
    return velocity_.Length();
}

double SpaceShip::Mass() {
    double volume = 0.0;
    double hull_volume = 1.4744;
    double fuel_volume = 0.0;

    if (engine_ != 0) {
        fuel_volume = engine_->FuelVolume();
    }
    volume = fuel_volume + hull_volume;

    return volume * density_;
}

void SpaceShip::UpdateGravity() {

    SolarSystemInterface* s = static_cast<SolarSystemInterface *>(OBJMGR.Get("solar"));
    if (s == 0) return;

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

void SpaceShip::OnDestroy() {
    destroyed_ = true;
    hud_->Disable();
    hotas_->Disable();
}

void SpaceShip::ProcessImpulse(float impulse) {

    hull_->ApplyImpact(impulse);
}

void SpaceShip::BeginContact(ContactInterface* object) {
    if (object->Type() != 'S') return;

    StationInterface *s = static_cast<StationInterface *>(object);

    if (s != 0 && ship_anchored_ == false) {
        station_iface_ = s;
    }
}

void SpaceShip::EndContact(ContactInterface* object) {
    StationInterface *s = static_cast<StationInterface *>(object);

    if (s != 0 && ship_anchored_ == false) {
        station_iface_ = 0;
    }
}

void SpaceShip::Init(b2World * world) {
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
    engine_->Mount(body_main_);

    radar_->Init(data_bus_);

    hud_->Init(data_bus_);
    hotas_->Init(data_bus_);
    hull_->Init(data_bus_);
}

void SpaceShip::CheckJoints(double delta_time) {
    if (ship_anchored_) return;
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

// BUG : Box2D cannot handle multiple rotations (>360.0).
void SpaceShip::NormalizeAngle(b2Body* b) {
    double a = b->GetAngle();
    if (a < 0.0) {
        double npi = -1.0 * floor(0.5 * a / M_PI);
        a += 2.0 * npi * M_PI;
        printf("a(%1.f) -> ", a);
    }
    a = fmod(a, 2.0 * M_PI);
    printf("fmod(a) = %.1f\n", a);
    b->SetTransform(b->GetPosition(), a);
}

void SpaceShip::NormalizeAngles() {

    NormalizeAngle(body_main_);
    NormalizeAngle(body_upper_);
    NormalizeAngle(body_left_);
    NormalizeAngle(body_right_);
    NormalizeAngle(body_left_gear_);
    NormalizeAngle(body_right_gear_);
}

void SpaceShip::Update(double delta_time) {
    // Check Joints
    CheckJoints(delta_time);
    // Gravity
    UpdateGravity();

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
        // NOTE : Should be published by navigation system (#115).
        // Used by HUD system.
        BD_Vector v;
        v.x = velocity_.x;
        v.y = velocity_.y;
        bus_connection_->Publish(db_ShipVelocity, &v);

        // NOTE : Should be published by a sensor device (#115).
        BD_Scalar av;
        av.value = angular_velocity_;
        bus_connection_->Publish(db_ShipAngularVelocity, &av);

        // NOTE : Should be published by a sensor device (#115).
        // Used by HUD & Radar systems.
        BD_Vector p;
        p.x = pos_main_.x;
        p.y = pos_main_.y;
        bus_connection_->Publish(db_ShipPosition, &p);

        // NOTE : Should be published by a sensor device (#115).
        // Used by HUD system.
        BD_Scalar s;
        s.value = angle_main_;
        bus_connection_->Publish(db_ShipAngle, &s);
    }

    engine_->Update(delta_time);
    radar_->Update(delta_time);
}

void SpaceShip::Render() {
    RenderShip();
    DISPLAY.UiMode();
    hud_->Render();
    if (station_iface_ != 0 && ship_anchored_) {
        station_iface_->RenderUi();
    }
}

void SpaceShip::RenderShip() {
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
    glLineWidth(2.0);
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

void SpaceShip::AnchorShip() {
    if (station_iface_ != 0) {
        NormalizeAngles();
        anchor_ = station_iface_->AnchorShip(body_left_gear_);
        if (anchor_ != 0) {
            ship_anchored_ = true;
            station_iface_->Connect();
        }
    }
}

void SpaceShip::ReleaseShip() {
    assert(station_iface_ != 0 && "No station interface!");
    station_iface_->Disconnect();
    world_->DestroyJoint( anchor_ );
    ship_anchored_ = false;
}

void SpaceShip::RefuelRequest() {
    if (station_iface_ != 0 && ship_anchored_) {
        station_iface_->Refuel(engine_);
    }
}

void SpaceShip::RepairRequest() {
    if (station_iface_ != 0 && ship_anchored_) {
        station_iface_->Repair(hull_);
    }
}

void SpaceShip::HotasInput(int key, bool action) {
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
        // NOTE : pass landing gear command to landing gear 'system' through hotas.
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
