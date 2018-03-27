#ifndef HOTAS_DEVICE_H_
#define HOTAS_DEVICE_H_

#include "game_data.h"
#include "ship_device.h"

class HOTASDevice : public ShipDevice {
public:
    HOTASDevice()
    : ShipDevice()
    {
    }
    ~HOTASDevice() {
    }
    void Init() {
    }
    void SetThrottle(double value) {
        GAMEDATA.SetThrust(20.0 * value, 0.0, 0.0);
    }
    void SetSteering(double value) {
        if (value < 0.0) {
            GAMEDATA.SetThrust(0.0, 0.0, 1.0); // -1.0
        }
        else if (value > 0.0) {
            GAMEDATA.SetThrust(0.0, 1.0, 0.0); //  1.0
        }
        else {
            GAMEDATA.SetThrust(0.0, 0.0, 0.0); //  0.0
        }
    }
    void ToggleLandingGear() {}
};

#endif // HOTAS_DEVICE_H_
