#ifndef BASIC_ENGINE_SYSTEM_H_
#define BASIC_ENGINE_SYSTEM_H_

#include "EffectsManager.h"
#include "EngineSystemInterface.h"
#include "GameDefinitions.h"

#include <assert.h>

// HOTAS sends commands here, and proper thrust forces
// and moments generated here, and told to physics.
class BasicEngineSystem : public EngineSystemInterface
{
public:
    BasicEngineSystem()
    : platform_body_(0)
    , thrust_{0.0, 0.0}
    , effects_(0)
    , kFuelVolumePerQuantity(0.1)
    , fuel_tank_size_(1.0)
    , remaining_fuel_(1.0)
    , main_thruster_(0.0)
    , left_thruster_(0.0)
    , right_thruster_(0.0)
    , stabilization_mode_(false)
    , angular_velocity_(0.0)
    {}
    ~BasicEngineSystem() {}

    void Mount(b2Body *body) {
        platform_body_ = body;
    }
    void ThrustForwardsCommand(double value) {
        if (remaining_fuel_ > 0.0) {
            main_thruster_ = 20.0 * value;
            left_thruster_ = 0.0;
            right_thruster_ = 0.0;
        }
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
    }
    void MomentCwCommand(double value) {
        if (remaining_fuel_ > 0.0) {
            left_thruster_ = value;
            main_thruster_ = 0.0;
            right_thruster_ = 0.0;
        }
        stabilization_mode_ = false;
    }
    void CancelMomentCommand() {
        StopThrusters();
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
        if (platform_body_ == 0) return;

        double a = platform_body_->GetAngle();
        a += 0.5 * M_PI;
        thrust_.x = main_thruster_ * cos(a);
        thrust_.y = main_thruster_ * sin(a);
        platform_body_->ApplyForceToCenter(thrust_, true);

        double torque = right_thruster_ - left_thruster_;
        platform_body_->ApplyTorque(torque, true);

        if (bus_connection_ != 0) {
            BD_Scalar thrust;
            double lf = GameDefinitions::LorentzFactor(platform_body_->GetLinearVelocity().Length());
            thrust.value = main_thruster_ * lf;
            bus_connection_->Publish(db_ShipThrust, &thrust);
        }

        b2Vec2 enginePos = platform_body_->GetPosition();
        effects_->MainThruster(thrust_, enginePos, platform_body_->GetLinearVelocity());
        effects_->BowThruster(torque, platform_body_->GetAngle() * 180.0 / M_PI, enginePos, platform_body_->GetLinearVelocity());
    }
    void Init(DataBus* bus) {

        EngineSystemInterface::Init(bus);

        DB_SUBSCRIBE(BasicEngineSystem, ShipAngularVelocity);

        effects_ = static_cast<EffectsManager*>(OBJMGR.Get("effects"));
        assert(effects_ != 0 && "effects not defined!");
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
                }
                else {
                    left_thruster_ = 0.0;
                    right_thruster_ = 0.0;
                }
            }
        }
        else {
            StopThrusters();
        }

        UpdateThrust();
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
    b2Body *platform_body_;
    b2Vec2 thrust_;
    EffectsManager *effects_;

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
