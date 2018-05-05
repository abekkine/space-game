#ifndef STATION_INTERFACE_H_
#define STATION_INTERFACE_H_

#include <GLFW/glfw3.h>
#include <Box2D.h>

#include "systems/engine_system_interface.h"
#include "systems/hull_system_interface.h"
#include "contact_interface.h"

class StationInterface : public ContactInterface {
public:
    virtual ~StationInterface() {}
    virtual void Init(b2World * world) = 0;
    virtual void Attach(b2Body * body) = 0;
    virtual b2Joint* AnchorShip(b2Body * body) = 0;
    virtual void Update(double time_step) = 0;
    virtual void Render() = 0;
    virtual void RenderUi() = 0;
    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual void Refuel(EngineSystemInterface *engine) = 0;
    virtual void Repair(HullSystemInterface *hull) = 0;

public:
    virtual void ProcessImpulse(float impulse) = 0;
    virtual void BeginContact(ContactInterface* object) = 0;
    virtual void EndContact(ContactInterface* object) = 0;

public:
    char Type() { return 'S'; }

protected:
    double x_, y_;
    double angle_;
    b2Body * body_;
    b2WeldJoint * joint_;
    b2World * world_;
};

#endif // STATION_INTERFACE_H_

