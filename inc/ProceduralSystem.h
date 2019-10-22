#ifndef PROCEDURAL_SYSTEM_H_
#define PROCEDURAL_SYSTEM_H_

#include "SolarSystemInterface.h"

#include <Box2D.h>

class Planet;
class StarInterface;

class ProceduralSystem : public SolarSystemInterface {
public:
    ProceduralSystem(b2World * world);
    ProceduralSystem();
    ~ProceduralSystem();
    void SetStar(StarInterface * star);
    b2Vec2 GetGravityAcceleration(b2Vec2 pos);
    void Init();
    void Update(double delta_time);
    void Render();
    std::vector<std::shared_ptr<Planet>> GetPlanets();

private:
    StarInterface * star_;
    std::vector<std::shared_ptr<Planet>> planets_;
    b2World * world_;
};

#endif // PROCEDURAL_SYSTEM_H_
