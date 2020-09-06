#include "BasicShipModel.h"
#include "GameDefinitions.h"
#include "DataBus.h"
#include "DataBusConnection.h"
#include "StationInterface.h"
#include "SolarSystemInterface.h"
#include "ObjectManager.h"
#include "Display.h"

#include <assert.h>

#include <memory>

BasicShipModel::BasicShipModel()
: kBreakForce(500.0)
, kDefaultSurfaceFriction(0.7)
, kVolume(1.4744)
, angle_main_(0.0)
, angle_upper_(0.0)
, angle_left_(0.0)
, angle_right_(0.0)
, angle_llg_(0)
, angle_rlg_(0)
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

, anchor_(0)
, color_{ 1.0, 1.0, 1.0 }
{
}

BasicShipModel::~BasicShipModel() {
}

void BasicShipModel::Init(b2World * world, void * user_data) {
    world_ = world;

    b2BodyDef bd;
    bd.type = b2_dynamicBody;

    b2FixtureDef fd;
    fd.density = density_;
    fd.friction = kDefaultSurfaceFriction;
    b2PolygonShape shape;

    // Main Body
    bd.position.Set(pos_main_.x, pos_main_.y);
    body_main_ = world_->CreateBody(&bd);
    body_main_->SetUserData(user_data);

    shape.Set(v_main_body_, NUM_PART_VERTICES);
    fd.shape = &shape;
    body_main_->CreateFixture(&fd);

    // Upper Body
    bd.position.Set(pos_main_.x, pos_main_.y + 0.5413);
    body_upper_ = world_->CreateBody(&bd);
    body_upper_->SetUserData(user_data);

    shape.Set(v_upper_body_, NUM_PART_VERTICES);
    fd.shape = &shape;
    body_upper_->CreateFixture(&fd);

    // Left Body
    bd.position.Set(pos_main_.x - 0.3228, pos_main_.y - 0.5632);
    body_left_ = world_->CreateBody(&bd);
    body_left_->SetUserData(user_data);

    shape.Set(v_left_body_, NUM_PART_VERTICES);
    fd.shape = &shape;
    body_left_->CreateFixture(&fd);

    // Right Body
    bd.position.Set(pos_main_.x + 0.3228, pos_main_.y - 0.5632);
    body_right_ = world_->CreateBody(&bd);
    body_right_->SetUserData(user_data);

    shape.Set(v_right_body_, NUM_PART_VERTICES);
    fd.shape = &shape;
    body_right_->CreateFixture(&fd);

    // Landing gear bodies
    bd.position.Set(pos_main_.x - 0.4361, pos_main_.y - 0.56);
    body_left_gear_ = world_->CreateBody(&bd);
    body_left_gear_->SetUserData(user_data);
    shape.Set(v_left_gear_, NUM_PART_VERTICES);
    fd.shape = &shape;
    body_left_gear_->CreateFixture(&fd);

    bd.position.Set(pos_main_.x + 0.4361, pos_main_.y - 0.56);
    body_right_gear_ = world_->CreateBody(&bd);
    body_right_gear_->SetUserData(user_data);
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
}

void BasicShipModel::Update(double delta_time) {
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
}

void BasicShipModel::UpdateGravity() {

    std::shared_ptr<SolarSystemInterface> s = std::static_pointer_cast<SolarSystemInterface>(OBJMGR.Get("solar"));
    if (s == 0) return;

    b2Vec2 g;
    g = s->GetGravityAcceleration(pos_main_);
    body_main_->ApplyForceToCenter(0.6085 * g, true);
    g_total_ = (0.6085 + 0.219 + 0.688 + 0.314) * g;

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
}

void BasicShipModel::Render() {
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

bool BasicShipModel::Dock(StationInterface * station) {
    bool result = false;
    if (station != 0) {
        NormalizeAngles();
        anchor_ = station->DockShip(body_left_gear_);
        if (anchor_ != 0) {
            result = true;
            station->Connect();
        }
    }
    return result;
}
bool BasicShipModel::Undock(StationInterface * station) {
    assert(station != 0 && "No station interface!");
    station->Disconnect();
    world_->DestroyJoint( anchor_ );
    anchor_ = 0;
    return false;
}

void BasicShipModel::LandingGear(uint8_t down) {
    if (down) {
        j_llg_->SetMotorSpeed(-0.3);
        j_rlg_->SetMotorSpeed(-0.3);
    }
    else {
        j_llg_->SetMotorSpeed(0.3);
        j_rlg_->SetMotorSpeed(0.3);
    }
}

double BasicShipModel::GetAngle() {
    return angle_main_;
}
void BasicShipModel::SetAngle(double angle) {
    angle_main_ = angle;
}
b2Vec2 const & BasicShipModel::GetPosition() {
    return pos_main_;
}
void BasicShipModel::SetPosition(double x, double y) {
    pos_main_.x = x;
    pos_main_.y = y;
}
double BasicShipModel::GetSpeed() {
    return velocity_.Length();
}
void BasicShipModel::GetVelocity(double & x, double & y) {
    x = velocity_.x;
    y = velocity_.y;
}
double BasicShipModel::GetMass() {
    return kVolume * density_;
}
double BasicShipModel::GetAngularVelocity() {
    return angular_velocity_;
}
void BasicShipModel::GetGravity(double & x, double & y) {
    x = g_total_.x;
    y = g_total_.y;
}
b2Body * BasicShipModel::GetEngineMount() {
    return body_main_;
}

void BasicShipModel::CheckJoints(double delta_time) {
    if (anchor_ != 0) return;
    double f1, f2, f3, f4;
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

// NOTE : Box2D cannot handle multiple rotations (>360.0).
//      : So normalize them into (-180, 180)
//      : https://www.iforce2d.net/b2dtut/rotate-to-angle
void BasicShipModel::NormalizeAngle(b2Body* b) {
    double a = b->GetAngle();
    while (a < -0.5 * M_PI) a += M_PI;
    while (a >  0.5 * M_PI) a -= M_PI;
    b->SetTransform(b->GetPosition(), a);
}

void BasicShipModel::NormalizeAngles() {

    NormalizeAngle(body_main_);
    NormalizeAngle(body_upper_);
    NormalizeAngle(body_left_);
    NormalizeAngle(body_right_);
    NormalizeAngle(body_left_gear_);
    NormalizeAngle(body_right_gear_);
}
