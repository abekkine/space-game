#include "HotasDevice.h"
#include "EngineSystemInterface.h"
#include "DataBus.h"

HOTASDevice::HOTASDevice()
{
    Enable();
    ship_engine_ = 0;
}

HOTASDevice::~HOTASDevice() {
}

void HOTASDevice::Init(DataBus * bus) {
    HotasSystemInterface::Init(bus);
}

void HOTASDevice::SetThrottle(double value) {
    if (ship_engine_ == 0 || active_ == false) {
        return;
    }

    ship_engine_->ThrustForwardsCommand(value);
}

void HOTASDevice::SetSteering(double value) {
    if (ship_engine_ == 0 || active_ == false) {
        return;
    }

    if (value < 0.0) {
        // Rotate Left
        ship_engine_->MomentCcwCommand(-value);
    }
    else if (value > 0.0) {
        // Rotate Right
        ship_engine_->MomentCwCommand(value);
    }
    else {
        // No rotate
        ship_engine_->CancelMomentCommand();
    }
}

void HOTASDevice::Stabilize() {
    if (ship_engine_ == 0 || active_ == false) {
        return;
    }

    ship_engine_->StabilizeRotation();
}

void HOTASDevice::ToggleLandingGear() {}
