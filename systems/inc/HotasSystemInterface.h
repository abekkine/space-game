#ifndef HOTAS_SYSTEM_INTERFACE_H_
#define HOTAS_SYSTEM_INTERFACE_H_

#include "ShipSystemBase.h"

#include <assert.h>

class HotasSystemInterface : public ShipSystemBase {
public:
    virtual ~HotasSystemInterface() {}
    virtual void SetThrottle(double value) = 0;
    virtual void SetSteering(double value) = 0;
    virtual void Stabilize() = 0;
    virtual void ToggleLandingGear() = 0;
};

#endif // HOTAS_SYSTEM_INTERFACE_H_

