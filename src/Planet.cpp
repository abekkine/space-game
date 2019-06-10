#include "Planet.h"
#include "StationInterface.h"
#include "GameDefinitions.h"
#include "PlanetStation.h"

#include <math.h>
#include <stdlib.h>

uint32_t Planet::counter_ = 0;

Planet::Planet()
: seed_(-1)
, kDefaultSurfaceFriction(0.7)
, x_(0.0)
, y_(0.0)
, angle_(0.0)
, angular_velocity_(0.0)
, density_(1.0)
, radius_(50.0)
, core_radius_(49.0)
, color_{1.0, 0.0, 0.0}
, station_(0)
, DL_surface_index_(-1)
, DL_core_index_(-1)
{
    identifier_ = counter_++;
}

Planet::~Planet() {
    delete station_;
}

void Planet::SetSeed(long int seed) {
    seed_ = seed;
    srand48(seed_);

    double height_factor = 0.2;
    if (seed_ == -1) {
        height_factor = 0.0;
    }

    DL_surface_index_ = glGenLists(1);
    glNewList(DL_surface_index_, GL_COMPILE);

    double R;
    const double a_step = 5.0 / (M_PI * radius_);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(0.0, 0.0);
    for (double a=0.0; a < 2.0 * M_PI; a+=a_step) {
        R = radius_ + drand48() * height_factor;
        glVertex2d(R * cos(a), R * sin(a));
    }
    glVertex2d(R, 0.0);
    glEnd();

    glEndList();

    DL_core_index_ = glGenLists(1);
    glNewList(DL_core_index_, GL_COMPILE);
    const double cR = core_radius_;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(0.0, 0.0);
    for (double a=0.0; a < 2.0 * M_PI; a+=a_step) {
        glVertex2d(cR * cos(a), cR * sin(a));
    }
    glVertex2d(cR, 0.0);
    glEnd();
    glEndList();
}

double Planet::Mass() {
    return M_PI * radius_ * radius_ * density_;
}

void Planet::SetPosition(double x, double y) {
    x_ = x;
    y_ = y;
}

void Planet::SetAngle(double value) {
    angle_ = value;
}

void Planet::SetAngularVelocity(double value) {
    angular_velocity_ = value;
}

void Planet::SetRadius(double value) {
    radius_ = value;
}

void Planet::SetCoreRadius(double value) {
    core_radius_ = value;
}

void Planet::SetColor(double value) {
    uint16_t colorbits = static_cast<uint16_t>(value);
    colorbits &= 0xfff;
    color_[0] = (colorbits >> 8) / 15.0;
    color_[1] = ((colorbits >> 4) & 0xf) / 15.0;
    color_[2] = (colorbits & 0xf) / 15.0;
}

void Planet::SetStation() {
    station_ = new PlanetStation();
    station_->Init(world_);
    station_->Attach(physics_body_);
}

b2Vec2 Planet::GetGravityAcceleration(b2Vec2 pos) {
    double dx = x_ - pos.x;
    double dy = y_ - pos.y;
    double r2 = dx*dx + dy*dy;
    double r = sqrt(r2);

    double gmr2 = GameDefinitions::kGravityConstant * Mass() / (r2 * r);
    double fx = gmr2 * dx;
    double fy = gmr2 * dy;

    return b2Vec2(fx, fy);
}

b2Vec2 Planet::GetDistance(double x, double y) {
    return b2Vec2(x_ - x, y_ - y);
}

double Planet::GetHorizonAngle(double x, double y) {
    b2Vec2 d(x_ - x, y_ - y);
    double sint = radius_ / d.Length();
    double t = asin(sint);
    return t;
}

void Planet::Init(b2World * world) {
    world_ = world;
    b2BodyDef def;
    def.type = b2_kinematicBody;
    def.position.Set(x_, y_);
    physics_body_ = world_->CreateBody(&def);

    b2CircleShape shape;
    // Circle position relative to body.
    shape.m_p.Set(0.0, 0.0);
    shape.m_radius = core_radius_;

    double r_rate = radius_ / core_radius_;
    b2FixtureDef fixture;
    fixture.density = density_ * r_rate * r_rate;
    fixture.friction = kDefaultSurfaceFriction;

    fixture.shape = &shape;
    physics_body_->CreateFixture(&fixture);
}

void Planet::Update() {
    physics_body_->SetAngularVelocity(angular_velocity_ * M_PI / 180.0);
    b2Vec2 pos = physics_body_->GetPosition();
    x_ = pos.x;
    y_ = pos.y;
    angle_ = physics_body_->GetAngle() * 180.0 / M_PI;
}

void Planet::Render() {
    glPushMatrix();
    glTranslated(x_, y_, 0.0);
    glRotated(angle_, 0.0, 0.0, 1.0);

    glColor3f(color_[0] * 0.9, color_[1] * 0.9, color_[2] * 0.9);
    glCallList(DL_surface_index_);
    glColor3fv(color_);
    glCallList(DL_core_index_);

    glPointSize(3.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    glVertex2d(0.0, radius_*0.99);
    glEnd();
    glPopMatrix();
    if (station_) {
        station_->Render();
    }
}
