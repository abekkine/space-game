#ifndef BASIC_ENGINE_SYSTEM_H_
#define BASIC_ENGINE_SYSTEM_H_

#include "engine_system_interface.h"

#include <assert.h>

// HOTAS sends commands here, and proper thrust forces
// and moments generated here, and told to physics.
class BasicEngineSystem : public EngineSystemInterface
{
public:
    BasicEngineSystem()
    : thrustUpdateHandler_(0)
    , momentUpdateHandler_(0)
    , kFuelVolumePerQuantity(0.1)
    , fuel_tank_size_(1.0)
    , remaining_fuel_(1.0)
    , main_thruster_(0.0)
    , left_thruster_(0.0)
    , right_thruster_(0.0)
    , stabilization_mode_(false)
    , angular_velocity_(0.0)
    {
        UpdateThrust();
    }
    ~BasicEngineSystem() {
        thrustUpdateHandler_ = 0;
        momentUpdateHandler_ = 0;
    }

    std::function<void(double)> thrustUpdateHandler_;
    void ThrustOutputHandler(std::function<void(double)> thrustOut) {
        thrustUpdateHandler_ = thrustOut;
    }
    std::function<void(double)> momentUpdateHandler_;
    void MomentOutputHandler(std::function<void(double)> momentOut) {
        momentUpdateHandler_ = momentOut;
    }
    void ThrustForwardsCommand(double value) {
        if (remaining_fuel_ > 0.0) {
            main_thruster_ = 20.0 * value;
            left_thruster_ = 0.0;
            right_thruster_ = 0.0;
        }
        UpdateThrust();
    }
    void ThrustBackwardsCommand(double value) {
        (void)value;
    }
    void MomentCcwCommand(double value) {
        if (remaining_fuel_ > 0.0) {
            right_thruster_ = value;
            main_thruster_ = 0.0;
            left_thruster_ = 0.0;
        }
        stabilization_mode_ = false;
        UpdateThrust();
    }
    void MomentCwCommand(double value) {
        if (remaining_fuel_ > 0.0) {
            left_thruster_ = value;
            main_thruster_ = 0.0;
            right_thruster_ = 0.0;
        }
        stabilization_mode_ = false;
        UpdateThrust();
    }
    void CancelMomentCommand() {
        StopThrusters();
        UpdateThrust();
    }
    void StopThrusters() {
        main_thruster_ = 0.0;
        left_thruster_ = 0.0;
        right_thruster_ = 0.0;
    }
    void ThrustPortCommand(double value) {
        (void)value;
    }
    void ThrustStarboardCommand(double value) {
        (void)value;
    }
    void StabilizeRotation() {
        stabilization_mode_ = true;
    }
    void UpdateThrust() {
        if (thrustUpdateHandler_ != 0) {
            thrustUpdateHandler_(main_thruster_);
        }

        if (momentUpdateHandler_ != 0) {
            momentUpdateHandler_(right_thruster_ - left_thruster_);
        }
    }
    void Init(DataBus* bus) {

        EngineSystemInterface::Init(bus);

        DB_SUBSCRIBE(BasicEngineSystem, ShipAngularVelocity);
    }
    void Update(double time_step) {
        const double fuel_consumption_rate = 0.001; // units per second
        if (remaining_fuel_ > 0.0) {
            remaining_fuel_ -= time_step * fuel_consumption_rate * main_thruster_;
            remaining_fuel_ -= time_step * fuel_consumption_rate * left_thruster_;
            remaining_fuel_ -= time_step * fuel_consumption_rate * right_thruster_;
            if (remaining_fuel_ < 0.0) {
                remaining_fuel_ = 0.0;
            }
            if (bus_connection_ != 0) {
                // Used by HUD system.
                BD_Scalar fuel;
                fuel.value = remaining_fuel_ / fuel_tank_size_;
                bus_connection_->Publish(db_ShipFuelQty, &fuel);
            }

            if (stabilization_mode_) {
                double aav = fabs(angular_velocity_);
                if (aav > 0.001) {
                    if (angular_velocity_ > 0.0) {
                        left_thruster_ = 0.8 * aav;
                        remaining_fuel_ -= time_step * fuel_consumption_rate * left_thruster_;
                    } else if(angular_velocity_ < 0.0) {
                        right_thruster_ = 0.8 * aav;
                        remaining_fuel_ -= time_step * fuel_consumption_rate * right_thruster_;
                    }
                    UpdateThrust();
                }
                else {
                    left_thruster_ = 0.0;
                    right_thruster_ = 0.0;
                    UpdateThrust();
                }
            }
        }
        else {
            StopThrusters();
            UpdateThrust();
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
    double FuelVolume() {

        return kFuelVolumePerQuantity * remaining_fuel_;
    }

private:
    void dbHandleShipAngularVelocity(BusDataInterface *data) {
        BD_Scalar *s = static_cast<BD_Scalar *>(data);
        if (s != 0) {
            angular_velocity_ = s->value;
        }
    }

private:
    const double kFuelVolumePerQuantity;
    double fuel_tank_size_;
    double remaining_fuel_;
    double main_thruster_;
    double left_thruster_;
    double right_thruster_;
    bool stabilization_mode_;
    double angular_velocity_;
};

#endif // BASIC_ENGINE_SYSTEM_H_
