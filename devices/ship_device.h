#ifndef SHIP_DEVICE_H_
#define SHIP_DEVICE_H_

#include "data_bus.h"

class ShipDevice {
public:
    ShipDevice() {
        bus_ = &DATABUS;
    }
    virtual ~ShipDevice() {}
    virtual void Init() = 0;
protected:
    DataBus * bus_;
};

#endif // SHIP_DEVICE_H_
