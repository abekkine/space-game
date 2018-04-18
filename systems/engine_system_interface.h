#ifndef ENGINE_SYSTEM_INTERFACE_H_
#define ENGINE_SYSTEM_INTERFACE_H_

#include "ship_system_interface.h"

#include <functional>

class EngineSystemInterface : public ShipSystemInterface {
public:
// Pilot commands
    virtual void MainThrustCommand(double value) = 0;
    virtual void ReverseThrustCommand(double value) = 0;
    virtual void RotateLeftCommand(double value) = 0;
    virtual void RotateRightCommand(double value) = 0;
    virtual void StopRotationCommand() = 0;
    virtual void StrafeLeftCommand(double value) = 0;
    virtual void StrafeRightCommand(double value) = 0;
// Fuel stuff
    virtual double Refuel(double value) = 0;
    virtual double DumpFuel(double value) = 0;
    virtual double FuelQuery() = 0;
// Output handlers
    virtual void ThrustOutputHandler(std::function<void(double)> thrustOut) = 0;
    virtual void MomentOutputHandler(std::function<void(double)> momentOut) = 0;

// Standard ship system interface
    virtual void Init() {}
    virtual void Update(double time_step) { (void)time_step; }
};

#endif // ENGINE_SYSTEM_INTERFACE_H_
