#ifndef SHIP_DEVICE_H_
#define SHIP_DEVICE_H_

#include "data_bus.h"

class ShipDevice {
public:
    virtual ~ShipDevice() {}
    // TODO : Instead of init Connect / Disconnect (bus) methods would be provided.
    virtual void Init(DataBus* bus) = 0;
protected:
    DataBus * bus_;
};

#endif // SHIP_DEVICE_H_
