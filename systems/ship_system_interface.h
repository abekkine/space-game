#ifndef SHIP_SYSTEM_INTERFACE_H_
#define SHIP_SYSTEM_INTERFACE_H_

class ShipSystemInterface {
public:
    virtual void Init() = 0;
    virtual void Update(double time_step) = 0;
};

#endif // SHIP_SYSTEM_INTERFACE_H_