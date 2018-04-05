#ifndef ENGINE_SYSTEM_INTERFACE_H_
#define ENGINE_SYSTEM_INTERFACE_H_

class EngineSystemInterface {
public:
    virtual void MainThrustCommand(double value) = 0;
    virtual void ReverseThrustCommand(double value) = 0;
    virtual void RotateLeftCommand(double value) = 0;
    virtual void RotateRightCommand(double value) = 0;
    virtual void StopRotationCommand() = 0;
    virtual void StrafeLeftCommand(double value) = 0;
    virtual void StrafeRightCommand(double value) = 0;

    virtual double Refuel(double value) = 0;
    virtual double DumpFuel(double value) = 0;
    virtual double FuelQuery() = 0;
};

#endif // ENGINE_SYSTEM_INTERFACE_H_
