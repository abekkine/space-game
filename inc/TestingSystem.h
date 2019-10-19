#ifndef TESTING_SYSTEM_H_
#define TESTING_SYSTEM_H_

#include "SolarSystemInterface.h"

#include <Box2D.h>

#include <vector>
#include <memory>

class Planet;

class TestingSystem : public SolarSystemInterface {
public:
    TestingSystem();
    ~TestingSystem();
    b2Vec2 GetGravityAcceleration(b2Vec2 pos);
    void Init(b2World * world);
    void Update(double delta_time);
    void Render();
    std::vector<std::shared_ptr<Planet>> GetPlanets();

private:
    std::vector<std::shared_ptr<Planet>> planets_;
};

#endif // TESTING_SYSTEM_H_
