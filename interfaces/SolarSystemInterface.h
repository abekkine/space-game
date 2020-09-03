#ifndef SOLAR_SYSTEM_INTERFACE_H_
#define SOLAR_SYSTEM_INTERFACE_H_

#include "StorableObject.h"

#include <Box2D/Box2D.h>

#include <vector>
#include <memory>

class Planet;
class StarInterface;

class SolarSystemInterface : public StorableObject {
public:
    virtual ~SolarSystemInterface() {}
    virtual void Init() = 0;
    virtual void SetStar(StarInterface * star) = 0;
    virtual void Render() = 0;
    virtual void Update(double time_step) = 0;
    virtual b2Vec2 GetGravityAcceleration(b2Vec2 pos) = 0;
    virtual std::vector<std::shared_ptr<Planet>> GetPlanets() = 0;
};

#endif // SOLAR_SYSTEM_INTERFACE_H_
