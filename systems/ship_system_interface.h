#ifndef SHIP_SYSTEM_INTERFACE_H_
#define SHIP_SYSTEM_INTERFACE_H_

#include "data_bus.h"

class ShipSystemInterface {
public:
    virtual void Init(DataBus* bus) = 0;
    virtual void Update(double time_step) = 0;
protected:
    DataBus* bus_;
};

#endif // SHIP_SYSTEM_INTERFACE_H_

