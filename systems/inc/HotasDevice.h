#ifndef HOTAS_DEVICE_H_
#define HOTAS_DEVICE_H_

#include "HotasSystemInterface.h"
#include "EngineSystemInterface.h"

class HOTASDevice : public HotasSystemInterface {
public:
    HOTASDevice()
    {
        Enable();
        ship_engine_ = 0;
    }
    ~HOTASDevice() {
    }
    void Init(DataBus * bus) {
        HotasSystemInterface::Init(bus);
    }
    void SetThrottle(double value) {
        if (ship_engine_ == 0 || active_ == false) {
            return;
        }

        ship_engine_->ThrustForwardsCommand(value);
    }
    void SetSteering(double value) {
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
    void Stabilize() {
        if (ship_engine_ == 0 || active_ == false) {
            return;
        }

        ship_engine_->StabilizeRotation();
    }
    void ToggleLandingGear() {}
};

#endif // HOTAS_DEVICE_H_
