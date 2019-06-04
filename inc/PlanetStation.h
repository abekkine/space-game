#ifndef PLANET_STATION_H_
#define PLANET_STATION_H_

#include "StationInterface.h"

#include <Box2D.h>

class Text;
class ContactInterface;
class EngineSystemInterface;
class HullSystemInterface;

class PlanetStation : public StationInterface {
public:
    PlanetStation();
    ~PlanetStation();
    void Init(b2World * world);
    void Attach(b2Body * body);
    b2Joint *AnchorShip(b2Body *body);
    void Connect();
    void Disconnect();
    void Refuel(EngineSystemInterface* engine);
    void Repair(HullSystemInterface* hull);
    void Update(double time_step);
    void RenderUi();
    void RenderStationServices();
    void Render();
    void ProcessImpulse(float impulse);
    void BeginContact(ContactInterface* object);
    void EndContact(ContactInterface* object);

private:
    b2Vec2 vertices_[6];
    Text *text_;
};

#endif // PLANET_STATION_H_

