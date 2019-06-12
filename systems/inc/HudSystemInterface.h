#ifndef HUD_SYSTEM_INTERFACE_H_
#define HUD_SYSTEM_INTERFACE_H_

#include "ShipSystemBase.h"

#include <assert.h>

class DataBus;
class DataBusInterface;

class HudSystemInterface : public ShipSystemBase {
public:
    virtual ~HudSystemInterface() {}
    // HuSI_01
    virtual void Render() = 0;
};

#endif // HUD_SYSTEM_INTERFACE_H_

