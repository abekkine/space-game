#ifndef BASIC_ENGINE_SYSTEM_H_
#define BASIC_ENGINE_SYSTEM_H_

#include "engine_system_interface.h"
#include "game_data.h"

// HOTAS sends commands here, and proper thrust forces
// and moments generated here, and told to physics.
class BasicEngineSystem : public EngineSystemInterface {
public:
    BasicEngineSystem() {}
    ~BasicEngineSystem() {}
    void MainThrustCommand(double value) {
        GAMEDATA.SetThrust(20.0 * value, 0.0, 0.0);
    }
    void ReverseThrustCommand(double value) {
        (void)value;
    }
    void RotateLeftCommand(double value) {
        GAMEDATA.SetThrust(0.0, 0.0, value); // -1.0
    }
    void RotateRightCommand(double value) {
        GAMEDATA.SetThrust(0.0, value, 0.0); //  1.0
    }
    void StopRotationCommand() {
        GAMEDATA.SetThrust(0.0, 0.0, 0.0); //  0.0
    }
    void StrafeLeftCommand(double value) {
        (void)value;
    }
    void StrafeRightCommand(double value) {
        (void)value;
    }

    double Refuel(double value) {
        double surplus_fuel;
        if (remaining_fuel_ < fuel_tank_size_) {
            if (value < fuel_tank_size_ - remaining_fuel_) {
                surplus_fuel = 0.0;
                remaining_fuel_ += value;
            }
            else {
                surplus_fuel = value - (fuel_tank_size_ - remaining_fuel_);
                remaining_fuel_ = fuel_tank_size_;
            }
        }
        else {
            surplus_fuel = value;
        }
        return surplus_fuel;
    }
    double DumpFuel(double value) {
        double dumped_amount;
        if (remaining_fuel_ > value) {
            remaining_fuel_ -= value;
            dumped_amount = value;
        }
        else {
            dumped_amount = remaining_fuel_;
            remaining_fuel_ = 0.0;
        }
        return dumped_amount;
    }
    double FuelQuery() {
        return remaining_fuel_;
    }

private:
    double fuel_tank_size_;
    double remaining_fuel_;
};

#endif // BASIC_ENGINE_SYSTEM_H_
#