#ifndef BASIC_ENGINE_SYSTEM_H_
#define BASIC_ENGINE_SYSTEM_H_

#include "ship_system_interface.h"
#include "engine_system_interface.h"
#include "game_data.h"
#include "data_bus.h"

// HOTAS sends commands here, and proper thrust forces
// and moments generated here, and told to physics.
class BasicEngineSystem : public EngineSystemInterface
{
public:
    BasicEngineSystem()
    : fuel_tank_size_(0.1)
    , remaining_fuel_(0.1)
    , main_thruster_(0.0)
    , left_thruster_(0.0)
    , right_thruster_(0.0)
    {
        UpdateThrust();
    }
    ~BasicEngineSystem() {}
    void MainThrustCommand(double value) {
        if (remaining_fuel_ > 0.0) {
            main_thruster_ = 20.0 * value;
            left_thruster_ = 0.0;
            right_thruster_ = 0.0;
        }
        UpdateThrust();
    }
    void ReverseThrustCommand(double value) {
        (void)value;
    }
    void RotateLeftCommand(double value) {
        if (remaining_fuel_ > 0.0) {
            right_thruster_ = value;
            main_thruster_ = 0.0;
            left_thruster_ = 0.0;
        }
        UpdateThrust();
    }
    void RotateRightCommand(double value) {
        if (remaining_fuel_ > 0.0) {
            left_thruster_ = value;
            main_thruster_ = 0.0;
            right_thruster_ = 0.0;
        }
        UpdateThrust();
    }
    void StopRotationCommand() {
        StopThrusters();
        UpdateThrust();
    }
    void StopThrusters() {
        main_thruster_ = 0.0;
        left_thruster_ = 0.0;
        right_thruster_ = 0.0;
    }
    void StrafeLeftCommand(double value) {
        (void)value;
    }
    void StrafeRightCommand(double value) {
        (void)value;
    }

    void UpdateThrust() {
        GAMEDATA.SetThrust(main_thruster_, left_thruster_, right_thruster_);
    }

    void Init() {}

    void Update(double time_step) {
        const double fuel_consumption_rate = 0.001; // units per second
        if (remaining_fuel_ > 0.0) {
            remaining_fuel_ -= time_step * fuel_consumption_rate * main_thruster_;
            remaining_fuel_ -= time_step * fuel_consumption_rate * left_thruster_;
            remaining_fuel_ -= time_step * fuel_consumption_rate * right_thruster_;
            if (remaining_fuel_ < 0.0) {
                remaining_fuel_ = 0.0;
            }
            BD_Scalar fuel;
            fuel.value = remaining_fuel_ / fuel_tank_size_;
            DATABUS.Publish(db_PlayerFuel, &fuel);
        }
        else {
            StopThrusters();
        }
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
    double main_thruster_;
    double left_thruster_;
    double right_thruster_;
};

#endif // BASIC_ENGINE_SYSTEM_H_
#
