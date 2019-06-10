#ifndef BASIC_ENGINE_MK1_H_
#define BASIC_ENGINE_MK1_H_

#include "EngineSystemInterface.h"

#include <Box2D.h>

class DataBus;
class BusDataInterface;
class EffectsManager;

// HOTAS sends commands here, and proper thrust forces
// and moments generated here, and told to physics.
class BasicEngineMk1 : public EngineSystemInterface
{
public:
    BasicEngineMk1();
    ~BasicEngineMk1();

    void Mount(b2Body *body);
    void ThrustForwardsCommand(double value);
    void MomentCcwCommand(double value);
    void MomentCwCommand(double value);
    void CancelMomentCommand();
    void StopThrusters();
    void UpdateThrust();
    void Init(DataBus* bus);
    void Update(double time_step);
    double Refuel(double value);
    double DumpFuel(double value);
    double FuelQuery();
    double FuelMass();

private:
    void dbHandleShipAngularVelocity(BusDataInterface *data);
    void dbHandleSteerCommand(BusDataInterface *data);
    void dbHandleThrottleCommand(BusDataInterface *data);
    void dbHandleStabilizeCommand(BusDataInterface *data);

private:
    b2Body *platform_body_;
    b2Vec2 thrust_;
    EffectsManager *effects_;

    const double kFuelMassPerQuantity;
    // units per second
    const double kFuelConsumptionRate;
    const double kEngineThrustFactor;
    double fuel_tank_size_;
    double remaining_fuel_;
    double main_thruster_;
    double left_thruster_;
    double right_thruster_;
    bool stabilization_mode_;
    double angular_velocity_;
};

#endif // BASIC_ENGINE_MK1_H_
