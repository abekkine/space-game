#include "ProceduralSystem.h"
#include "ObjectManager.h"
#include "Planet.h"
#include "StarInterface.h"

#include <GLFW/glfw3.h>

// DEBUG
#include <iostream>

ProceduralSystem::ProceduralSystem(StarInterface * star)
: star_(star)
{
    // DEBUG
    double sx, sy, r, m;
    star_->GetPosition(sx, sy);
    r = star_->GetRadius();
    m = star_->GetMass();
    std::cout << sx << ", " << sy << ", " << r << ", " << m << '\n';
}

ProceduralSystem::ProceduralSystem()
: ProceduralSystem(0) {}

ProceduralSystem::~ProceduralSystem() {}

b2Vec2 ProceduralSystem::GetGravityAcceleration(b2Vec2 pos) {
    b2Vec2 g(0.0, 0.0);
    g += planets_[0]->GetGravityAcceleration(pos);
    return g;
}

void ProceduralSystem::Init(b2World * world) {
    std::shared_ptr<Planet> p = std::make_shared<Planet>();
    p->SetPosition(-5000.0, -500.0);
    p->SetAngle(0.0);
    double radius = star_->GetRadius();
    p->SetRadius(radius);
    p->SetCoreRadius(100.0);
    p->SetAngularVelocity(1.0);
    float * pColor = star_->GetColor();
    p->SetColor(pColor[0], pColor[1], pColor[2]);
    p->SetSeed(-1);

    planets_.push_back(p);

    p->Init(world);
}

void ProceduralSystem::Update(double delta_time) {
    (void)delta_time;
    planets_[0]->Update();
}

void ProceduralSystem::Render() {
    glPushMatrix();
    planets_[0]->Render();
    glPopMatrix();
}

std::vector<std::shared_ptr<Planet>> ProceduralSystem::GetPlanets() {
    return planets_;
}
