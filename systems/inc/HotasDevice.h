#ifndef HOTAS_DEVICE_H_
#define HOTAS_DEVICE_H_

#include "HotasSystemInterface.h"

class DataBus;

class HOTASDevice : public HotasSystemInterface {
public:
    HOTASDevice();
    ~HOTASDevice();
    void SetThrottle(double value);
    void SetSteering(double value);
    void Stabilize();
    void ToggleLandingGear();
};

#endif // HOTAS_DEVICE_H_
