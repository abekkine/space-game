#ifndef SHIP_MODEL_INTERFACE_H
#define SHIP_MODEL_INTERFACE_H

#include "StorableObject.h"
#include "StationInterface.h"

#include <Box2D/Box2D.h>
#include <stdint.h>

class ShipModelInterface : public StorableObject {
public:
    virtual ~ShipModelInterface() {}

    virtual void Init(b2World * world, void * user_data) = 0;   // SMI_01
    virtual void Update(double delta_time) = 0;                 // SMI_02
    virtual void Render() = 0;                                  // SMI_03
    virtual bool Dock(StationInterface * station) = 0;          // SMI_04
    virtual bool Undock(StationInterface * station) = 0;        // SMI_05
    virtual void LandingGear(uint8_t down) = 0;                 // SMI_06
    virtual double GetAngle() = 0;                              // SMI_07
    virtual void SetAngle(double angle) = 0;                    // SMI_08
    virtual b2Vec2 const & GetPosition() = 0;                   // SMI_09
    virtual void SetPosition(double x, double y) = 0;           // SMI_10
    virtual double GetSpeed() = 0;                              // SMI_11
    virtual void GetVelocity(double & x, double & y) = 0;       // SMI_12
    virtual double GetMass() = 0;                               // SMI_13
    virtual double GetAngularVelocity() = 0;                    // SMI_14
    virtual void GetGravity(double & x, double & y) = 0;        // SMI_15
    virtual b2Body * GetEngineMount() = 0;                      // SMI_16
};

#endif // SHIP_MODEL_INTERFACE_H
