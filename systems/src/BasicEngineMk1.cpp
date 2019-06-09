#include "BasicEngineMk1.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "EffectsManager.h"
#include "GameDefinitions.h"
#include "ObjectManager.h"

#include <assert.h>

// HOTAS sends commands here, and proper thrust forces
// and moments generated here, and told to physics.
BasicEngineMk1::BasicEngineMk1()
: platform_body_(0)
, thrust_{0.0, 0.0}
, effects_(0)
, kFuelMassPerQuantity(0.1)
, fuel_tank_size_(1.0)
, remaining_fuel_(1.0)
, main_thruster_(0.0)
, left_thruster_(0.0)
, right_thruster_(0.0)
, stabilization_mode_(false)
, angular_velocity_(0.0)
{}

BasicEngineMk1::~BasicEngineMk1() {}

void BasicEngineMk1::Mount(b2Body *body) {
    platform_body_ = body;
}

void BasicEngineMk1::ThrustForwardsCommand(double value) {
    if (remaining_fuel_ > 0.0) {
        main_thruster_ = 20.0 * value;
        left_thruster_ = 0.0;
        right_thruster_ = 0.0;
    }
}

void BasicEngineMk1::MomentCcwCommand(double value) {
    if (remaining_fuel_ > 0.0) {
        right_thruster_ = value;
        main_thruster_ = 0.0;
        left_thruster_ = 0.0;
    }
    stabilization_mode_ = false;
}

void BasicEngineMk1::MomentCwCommand(double value) {
    if (remaining_fuel_ > 0.0) {
        left_thruster_ = value;
        main_thruster_ = 0.0;
        right_thruster_ = 0.0;
    }
    stabilization_mode_ = false;
}

void BasicEngineMk1::CancelMomentCommand() {
    StopThrusters();
}

void BasicEngineMk1::StopThrusters() {
    main_thruster_ = 0.0;
    left_thruster_ = 0.0;
    right_thruster_ = 0.0;
}

void BasicEngineMk1::UpdateThrust() {
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

void BasicEngineMk1::Init(DataBus* bus) {

    EngineSystemInterface::Init(bus);

    DB_SUBSCRIBE(BasicEngineMk1, ShipAngularVelocity);
    DB_SUBSCRIBE(BasicEngineMk1, SteerCommand);
    DB_SUBSCRIBE(BasicEngineMk1, ThrottleCommand);
    DB_SUBSCRIBE(BasicEngineMk1, StabilizeCommand);

    effects_ = static_cast<EffectsManager*>(OBJMGR.Get("effects"));
    assert(effects_ != 0 && "effects not defined!");
}

void BasicEngineMk1::Update(double time_step) {
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

double BasicEngineMk1::Refuel(double value) {
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

double BasicEngineMk1::DumpFuel(double value) {
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

double BasicEngineMk1::FuelQuery() {
    return remaining_fuel_;
}

double BasicEngineMk1::FuelMass() {

    return kFuelMassPerQuantity * remaining_fuel_;
}

void BasicEngineMk1::dbHandleShipAngularVelocity(BusDataInterface *data) {
    BD_Scalar *s = static_cast<BD_Scalar *>(data);
    if (s != 0) {
        angular_velocity_ = s->value;
    }
}

void BasicEngineMk1::dbHandleSteerCommand(BusDataInterface *data) {
    BD_Scalar *s = static_cast<BD_Scalar *>(data);
    if (s == 0) return;

    if (s->value < 0.0) {
        MomentCcwCommand(-s->value);
    }
    else if (s->value > 0.0) {
        MomentCwCommand( s->value);
    }
    else {
        CancelMomentCommand();
    }
}

void BasicEngineMk1::dbHandleThrottleCommand(BusDataInterface *data) {
    BD_Scalar *s = static_cast<BD_Scalar *>(data);
    if (s == 0) return;

    ThrustForwardsCommand(s->value);
}

void BasicEngineMk1::dbHandleStabilizeCommand(BusDataInterface *data) {  
    (void)data;
    stabilization_mode_ = true;
}


