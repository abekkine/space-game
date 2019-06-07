#include "BasicHullSystem.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"

BasicHullSystem::BasicHullSystem()
{
    on_destroyed_ = 0;
}

BasicHullSystem::~BasicHullSystem() {}

void BasicHullSystem::Init(DataBus * bus) {
    HullSystemInterface::Init(bus);

    max_integrity_ = 10.0;
    integrity_ = max_integrity_;
    resistance_factor_ = 1.0;
    damage_threshold_ = 1.0;
}

void BasicHullSystem::SetDestructionCallback(std::function<void()> cb) {
    on_destroyed_ = cb;
}

void BasicHullSystem::ApplyImpact(double impulse) {
    if (integrity_ > 0.0 && impulse > damage_threshold_) {
        integrity_ -= resistance_factor_ * impulse;
        if (integrity_ <= 0.0) {
            on_destroyed_();
        }
        else if (bus_connection_ != 0) {
            // Used by HUD system.
            BD_Scalar damage_ratio;
            damage_ratio.value = integrity_ / max_integrity_;
            bus_connection_->Publish(db_ShipDamage, &damage_ratio);
        }
    }
}

double BasicHullSystem::Repair(double value) {
    double surplus_repair_points;
    if (integrity_ < max_integrity_) {
        if (value < (max_integrity_ - integrity_)) {
            surplus_repair_points = 0.0;
            integrity_ += value;
        }
        else {
            surplus_repair_points = value - (max_integrity_ - integrity_);
            integrity_ = max_integrity_;
        }
        if (bus_connection_ != 0) {
            // Used by HUD system.
            BD_Scalar damage_ratio;
            damage_ratio.value = integrity_ / max_integrity_;
            bus_connection_->Publish(db_ShipDamage, &damage_ratio);
        }
    }
    else {
        surplus_repair_points = value;
    }
    return surplus_repair_points;
}
