#ifndef SPACE_SHIP_H_
#define SPACE_SHIP_H_

#include "ContactInterface.h"

#include <Box2D.h>

class DataBus;
class DataBusConnection;
class HudSystemInterface;
class HotasSystemInterface;
class EngineSystemInterface;
class HullSystemInterface;
class StationInterface;
class ShipModelInterface;
class ShipSystemBase;

class SpaceShip : public ContactInterface {
public: // Contact interface
    void ProcessImpulse(float impulse);             // CI_01
    void BeginContact(ContactInterface* object);    // CI_02
    void EndContact(ContactInterface* object);      // CI_03
    char Type();                                    // CI_04

public:
    SpaceShip();
    ~SpaceShip();
    void Init(b2World * world);
    void SetAngle(double angle);
    double GetAngle();
    void SetPosition(double x, double y);
    b2Vec2 const & GetPosition();
    double GetSpeed();
    double Mass();
    void UpdateGravity();
    void OnDestroy();
    void Update(double delta_time);
    void Render();
    void RenderShip();
    void RefuelRequest();
    void RepairRequest();
    void HotasInput(int key, bool action);

private:
    ShipModelInterface * model_;
    // Ship communication bus
    DataBus * data_bus_;
    // Ship Systems
    // -- HUD system
    HudSystemInterface * hud_;
    // -- HOTAS system
    HotasSystemInterface * hotas_;
    // -- Engine system
    EngineSystemInterface * engine_;
    // -- Radar system
    ShipSystemBase * radar_;
    // -- Jump drive
    ShipSystemBase * jump_drive_;
    // -- Hull system
    HullSystemInterface * hull_;
    // -- Sensor system
    ShipSystemBase * sensor_;

    // Landing gear up/down control variable.
    uint8_t landing_gear_state_;
    StationInterface* station_iface_;
    bool ship_anchored_;
    // Ship destruction flag.
    bool destroyed_;
};

#endif // SPACE_SHIP_H_
