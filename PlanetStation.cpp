#include "PlanetStation.h"

#include "ContactInterface.h"
#include "EngineSystemInterface.h"
#include "HullSystemInterface.h"
#include "Text.h"
#include "Display.h"

#include <stdio.h>


PlanetStation::PlanetStation()
: vertices_{
    {-1.3, 0.5},
    {-1.7, 0.3},
    {-1.7, -1.2},
    {1.7, -1.2},
    {1.7, 0.3},
    {1.3, 0.5}}
, text_(0)
{}

PlanetStation::~PlanetStation() {
    delete text_;
}

void PlanetStation::Init(b2World * world) {
    world_ = world;
    text_ = new Text();
    text_->AddFont("menu", "../fonts/mono/roboto.ttf");
    text_->UseFont("menu", 24);
    x_ = 0.0;
    y_ = 100.0;
    angle_ = 0.0;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(x_, y_);
    body_ = world_->CreateBody(&bd);
    body_->SetUserData(this);
    
    b2PolygonShape shape;
    shape.Set(vertices_, 6);
    body_->CreateFixture(&shape, 1.0f);
}

void PlanetStation::Attach(b2Body * body) {
    b2WeldJointDef wjd;
    wjd.bodyA = body;
    wjd.bodyB = body_;

    b2Vec2 worldAnchor = body_->GetWorldCenter();
    wjd.Initialize(body, body_, worldAnchor);
    wjd.localAnchorA = body->GetLocalPoint(worldAnchor);
    wjd.localAnchorB = body_->GetLocalPoint(worldAnchor);
    wjd.referenceAngle = 0.0;
    wjd.collideConnected = false;

    joint_ = (b2WeldJoint *) world_->CreateJoint(&wjd);
}

b2Joint *PlanetStation::DockShip(b2Body *body) {
    b2WeldJointDef wjd;
    wjd.bodyA = body;
    wjd.bodyB = body_;

    b2Vec2 worldAnchor = body_->GetWorldCenter();
    wjd.Initialize(body, body_, worldAnchor);
    wjd.localAnchorA = body->GetLocalPoint(worldAnchor);
    wjd.localAnchorB = body_->GetLocalPoint(worldAnchor);
    wjd.referenceAngle = 0.0;
    wjd.collideConnected = false;

    b2Joint *anchor = world_->CreateJoint(&wjd);
    return anchor;
}

void PlanetStation::Connect() {
    puts("Welcome to ... station!");
}

void PlanetStation::Disconnect() {
    puts("Leaving ... station!");
}

void PlanetStation::Refuel(EngineSystemInterface* engine) {

    engine->Refuel(5.0);
}

void PlanetStation::Repair(HullSystemInterface* hull) {

    hull->Repair(5.0);
}

void PlanetStation::Update(double time_step) {
    (void)time_step;
    b2Vec2 pos = body_->GetPosition();
    x_ = pos.x;
    y_ = pos.y;
    angle_ = body_->GetAngle() * 180.0 / M_PI;
}

void PlanetStation::RenderUi() {
    const int planet_ui_width = 500;
    const int planet_ui_height = 200;
    const int planet_ui_x = 10;
    const int planet_ui_y = 10;
    const float planet_ui_fill_color[4] = { 0.0, 0.0, 0.5, 0.8 };
    const float planet_ui_line_color[4] = { 1.0, 1.0, 1.0, 0.2 };
    const float planet_ui_text_color[4] = { 1.0, 1.0, 1.0, 1.0 };
    const float planet_ui_line_width = 3.0;

    glPushMatrix();
    glLoadIdentity();
    glTranslated(planet_ui_x, planet_ui_y, 0.0);

    // Background
    glColor4fv(planet_ui_fill_color);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(planet_ui_width, 0);
    glVertex2i(planet_ui_width, planet_ui_height);
    glVertex2i(0, planet_ui_height);
    glEnd();

    // Frame
    glLineWidth(planet_ui_line_width);
    glColor4fv(planet_ui_line_color);
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 0);
    glVertex2i(planet_ui_width, 0);
    glVertex2i(planet_ui_width, planet_ui_height);
    glVertex2i(0, planet_ui_height);
    glEnd();

    // Text Menu
    glColor4fv(planet_ui_text_color);
    RenderStationServices();
    glPopMatrix();
}

void PlanetStation::RenderStationServices() {

    text_->Render(20.0, 35.0, "Re[F]uel");
    text_->Render(20.0, 65.0, "[R]epair");
}

void PlanetStation::Render() {
    glPushMatrix();
    glTranslated(x_, y_, 0.0);
    glRotated(angle_, 0.0, 0.0, 1.0);
    glColor3f(0.8, 0.8, 1.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(0.0, 0.0);
    for (int i=0; i<6; ++i) {
        glVertex2d(vertices_[i].x, vertices_[i].y);
    }
    glVertex2d(vertices_[0].x, vertices_[0].y);
    glEnd();

    #ifdef DEBUG_SERVICE_ZONE
    glColor3f(1.0, 0.0, 0.0);
    const double r_zone = 1.4;
    glBegin(GL_LINE_LOOP);
    for (double a=0.0; a<=2.0*M_PI; a+= 0.5) {
        glVertex2d(r_zone * cos(a), r_zone * sin(a));
    }
    glEnd();
    #endif // DEBUG_SERVICE_ZONE

    glPopMatrix();

    Update(0.02);
}

void PlanetStation::ProcessImpulse(float impulse) {
    (void)impulse;
}

void PlanetStation::BeginContact(ContactInterface* object) {
    (void)object;
}

void PlanetStation::EndContact(ContactInterface* object) {
    (void)object;
}
