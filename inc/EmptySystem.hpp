#ifndef EMPTY_SYSTEM_HPP_
#define EMPTY_SYSTEM_HPP_

#include "SolarSystemInterface.h"
#include "ObjectManager.h"

class EmptySystem : public SolarSystemInterface {
public:
    EmptySystem()
    {}
    ~EmptySystem() {}
    b2Vec2 GetGravityAcceleration(b2Vec2 pos) {
        b2Vec2 g(0.0, 0.0);
        return g;
    }
    void Init(b2World * world) {
        (void)world;
    }
    void Update(double delta_time) {
        (void)delta_time;
    }
    void Render() {}
    std::vector<std::shared_ptr<Planet>> GetPlanets() {
        return planets_;
    }

private:
    std::vector<std::shared_ptr<Planet>> planets_;
};

#endif // EMPTY_SYSTEM_HPP_
