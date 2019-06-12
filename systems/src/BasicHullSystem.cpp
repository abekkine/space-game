#include "BasicHullSystem.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"

BasicHullSystem::BasicHullSystem()
: kMaxIntegrity(10.0)
, kResistanceFactor(1.0)
, kDamageThreshold(1.0)
{
    name_ = "hull";
    on_destroyed_ = 0;
    integrity_ = kMaxIntegrity;
}

BasicHullSystem::~BasicHullSystem() {}

void BasicHullSystem::SetDestructionCallback(std::function<void()> cb) {
    on_destroyed_ = cb;
}

void BasicHullSystem::ApplyImpact(double impulse) {
    if (integrity_ > 0.0 && impulse > kDamageThreshold) {
        integrity_ -= kResistanceFactor * impulse;
        if (integrity_ <= 0.0) {
            on_destroyed_();
        }
        else if (bus_connection_ != 0) {
            // Used by HUD system.
            BD_Scalar damage_ratio;
            damage_ratio.value = integrity_ / kMaxIntegrity;
            bus_connection_->Publish(db_ShipDamage, &damage_ratio);
        }
    }
}

double BasicHullSystem::Repair(double value) {
    double surplus_repair_points;
    if (integrity_ < kMaxIntegrity) {
        if (value < (kMaxIntegrity - integrity_)) {
            surplus_repair_points = 0.0;
            integrity_ += value;
        }
        else {
            surplus_repair_points = value - (kMaxIntegrity - integrity_);
            integrity_ = kMaxIntegrity;
        }
        if (bus_connection_ != 0) {
            // Used by HUD system.
            BD_Scalar damage_ratio;
            damage_ratio.value = integrity_ / kMaxIntegrity;
            bus_connection_->Publish(db_ShipDamage, &damage_ratio);
        }
    }
    else {
        surplus_repair_points = value;
    }
    return surplus_repair_points;
}
