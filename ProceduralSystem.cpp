#include "ProceduralSystem.h"
#include "ObjectManager.h"
#include "Planet.h"
#include "StarInterface.h"
#include "RandomGenerator.hpp"
#include "Display.h"

// DEBUG
#include <stdlib.h>
#include <iostream>

ProceduralSystem::ProceduralSystem(b2World * world)
: world_(world)
{
}

ProceduralSystem::ProceduralSystem()
: ProceduralSystem(0) {}

ProceduralSystem::~ProceduralSystem() {}

void ProceduralSystem::SetStar(StarInterface * star) {
    star_ = star;
    planets_.clear();

    Init();
}

b2Vec2 ProceduralSystem::GetGravityAcceleration(b2Vec2 pos) {
    b2Vec2 g(0.0, 0.0);
    g += planets_[0]->GetGravityAcceleration(pos);
    return g;
}

void ProceduralSystem::Init() {
    std::shared_ptr<Planet> p = std::make_shared<Planet>();
    RandomGenerator RG;
    RG.SetRange(0.0, 2.0 * 3.141592653);
    double alpha = RG.GetValue();
    // double alpha = drand48() * 2.0 * 3.14159;
    p->SetPosition(5000.0 * cos(alpha), 5000.0 * sin(alpha));
    p->SetAngle(0.0);
    double radius = star_->GetRadius();
    p->SetRadius(radius);
    p->SetCoreRadius(100.0);
    p->SetAngularVelocity(1.0);
    float * pColor = star_->GetColor();
    p->SetColor(pColor[0], pColor[1], pColor[2]);
    p->SetSeed(-1);

    planets_.push_back(p);

    p->Init(world_);
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
