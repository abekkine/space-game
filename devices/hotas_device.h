#ifndef HOTAS_DEVICE_H_
#define HOTAS_DEVICE_H_

#include "ship_device.h"

#include "systems/engine_system_interface.h"
#include "systems/ship_systems_manager.h"

class HOTASDevice : public ShipDevice {
public:
    HOTASDevice()
    : ShipDevice()
    , ship_engine_(0)
    {
    }
    ~HOTASDevice() {
    }
    void Init() {
        ship_engine_ = SYSTEMSMGR.getEngineSystem();
    }
    void SetThrottle(double value) {
        if (ship_engine_ == 0) {
            return;
        }

        ship_engine_->MainThrustCommand(value);
    }
    void SetSteering(double value) {
        if (ship_engine_ == 0) {
            return;
        }

        if (value < 0.0) {
            // Rotate Left
            ship_engine_->RotateLeftCommand(-value);
        }
        else if (value > 0.0) {
            // Rotate Right
            ship_engine_->RotateRightCommand(value);
        }
        else {
            // No rotate
            ship_engine_->StopRotationCommand();
        }
    }
    void ToggleLandingGear() {}

private:
    EngineSystemInterface* ship_engine_;
};

#endif // HOTAS_DEVICE_H_
