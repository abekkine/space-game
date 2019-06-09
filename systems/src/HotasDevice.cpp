#include "HotasDevice.h"
#include "EngineSystemInterface.h"
#include "BusDataTypes.h"
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
    if (active_ == false) {
        return;
    }

    BD_Scalar throttle;
    throttle.value = value;
    bus_connection_->Publish(db_ThrottleCommand, &throttle);
}

void HOTASDevice::SetSteering(double value) {
    if (active_ == false) {
        return;
    }

    BD_Scalar steering;
    steering.value = value;
    bus_connection_->Publish(db_SteerCommand, &steering);
}

void HOTASDevice::Stabilize() {
    if (active_ == false) {
        return;
    }

    bus_connection_->Publish(db_StabilizeCommand, 0);
}

void HOTASDevice::ToggleLandingGear() {}