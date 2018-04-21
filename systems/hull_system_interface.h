#ifndef HULL_SYSTEM_INTERFACE_H_
#define HULL_SYSTEM_INTERFACE_H_

#include "ship_system_interface.h"

#include <assert.h>

class HullSystemInterface : public ShipSystemInterface {
public:
    virtual ~HullSystemInterface() {}
public:
    virtual void Init(DataBus * bus) {
        assert(bus != 0);
        bus_ = bus;
    }
    virtual void ApplyImpact(double impulse) = 0;
    virtual void SetDestructionCallback(std::function<void()> cb) = 0;
protected:
    std::function<void()> on_destroyed_;
};

#endif // HULL_SYSTEM_INTERFACE_H_

