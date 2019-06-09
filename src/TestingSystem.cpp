#include "TestingSystem.h"
#include "ObjectManager.h"
#include "Planet.h"

#include <GLFW/glfw3.h>

TestingSystem::TestingSystem()
: kNumPlanets(5)
, planets_(0)
{}

TestingSystem::~TestingSystem() {

    OBJMGR.Remove("planets");
    OBJMGR.Remove("nplanets");

    delete [] planets_;
}

b2Vec2 TestingSystem::GetGravityAcceleration(b2Vec2 pos) {
    b2Vec2 g(0.0, 0.0);
    for (int i=0; i<kNumPlanets; ++i) {

        g += planets_[i].GetGravityAcceleration(pos);
    }
    return g;
}


void TestingSystem::Init(b2World * world) {
    // Instantiate planets.
    planets_ = new Planet[kNumPlanets];

    CelestialBodyDef u[] = {
        {   3,      0.0,      0.0, 0.0,  100.0,  99.7,  0.20,  435.0 }, // Earth
        {  99,    600.0,      0.0, 0.0,   25.0,  24.9,  0.01, 2730.0 }, // Moon
        {  -1, -19900.0, -11500.0, 0.0, 1000.0, 100.0,  1.00, 3984.0 }, // Sun
        { 123,  -4900.0, -11500.0, 0.0,   90.0,  89.9,  0.10, 1742.0 }, // Venus
        {  49,  13000.0, -11500.0, 0.0,   50.0,  49.9,  0.15, 3172.0 }  // Mars
    };

    for (int i=0; i<kNumPlanets; ++i) {
        planets_[i].SetPosition(u[i].x, u[i].y);
        planets_[i].SetAngle(u[i].angle);
        planets_[i].SetRadius(u[i].radius);
        planets_[i].SetCoreRadius(u[i].coreRadius);
        planets_[i].SetAngularVelocity(u[i].angularVelocity);
        planets_[i].SetColor(u[i].color);
        planets_[i].SetSeed(u[i].seed);
    }

    // TODO : (#146) Planets & numPlanets would be passed inside a single object.
    OBJMGR.Set("planets", planets_);
    OBJMGR.Set("nplanets", (void *)&kNumPlanets);

    // Planet creation
    for(int i=0; i<kNumPlanets; ++i) {
        planets_[i].Init(world);
        if (i == 0) {
            planets_[i].SetStation();
        }
    }
}

void TestingSystem::Update(double delta_time) {
    (void)delta_time;
    for (int i=0; i<kNumPlanets; ++i) {
        planets_[i].Update();
    }
}

void TestingSystem::Render() {
    for (int i=0; i<kNumPlanets; ++i) {
        glPushMatrix();
        planets_[i].Render();
        glPopMatrix();
    }
}