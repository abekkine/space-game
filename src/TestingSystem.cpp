#include "TestingSystem.h"
#include "ObjectManager.h"
#include "Planet.h"
#include "CelestialBodyDefinition.h"

#include <GLFW/glfw3.h>

TestingSystem::TestingSystem()
{}

TestingSystem::~TestingSystem() {}

b2Vec2 TestingSystem::GetGravityAcceleration(b2Vec2 pos) {
    b2Vec2 g(0.0, 0.0);
    for (auto p : planets_) {
        g += p->GetGravityAcceleration(pos);
    }
    return g;
}


void TestingSystem::Init(b2World * world) {
    // Instantiate planets.
    const int numPlanets = 5;
    CelestialBodyDefinition u[numPlanets] = {
        {   3,      0.0,      0.0, 0.0,  100.0,  99.7,  0.20,  435.0 }, // Earth
        {  99,    600.0,      0.0, 0.0,   25.0,  24.9,  0.01, 2730.0 }, // Moon
        {  -1, -19900.0, -11500.0, 0.0, 1000.0, 100.0,  1.00, 3984.0 }, // Sun
        { 123,  -4900.0, -11500.0, 0.0,   90.0,  89.9,  0.10, 1742.0 }, // Venus
        {  49,  13000.0, -11500.0, 0.0,   50.0,  49.9,  0.15, 3172.0 }  // Mars
    };

    for (int i=0; i<numPlanets; ++i) {
        std::shared_ptr<Planet> p = std::make_shared<Planet>();
        p->SetPosition(u[i].x, u[i].y);
        p->SetAngle(u[i].angle);
        p->SetRadius(u[i].radius);
        p->SetCoreRadius(u[i].coreRadius);
        p->SetAngularVelocity(u[i].angularVelocity);
        p->SetColor(u[i].color);
        p->SetSeed(u[i].seed);
        p->Init(world);
        if (i == 0) {
            p->SetStation();
        }
        planets_.push_back(p);
    }
}

void TestingSystem::Update(double delta_time) {
    (void)delta_time;
    for (auto p : planets_) {
        p->Update();
    }
}

void TestingSystem::Render() {
    for (auto p : planets_) {
        glPushMatrix();
        p->Render();
        glPopMatrix();
    }
}

std::vector<std::shared_ptr<Planet>> TestingSystem::GetPlanets() {
    return planets_;
}
