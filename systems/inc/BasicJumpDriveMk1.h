#ifndef BASIC_JUMP_DRIVER_MK1_H_
#define BASIC_JUMP_DRIVER_MK1_H_

#include "ShipSystemBase.h"
#include "StarInterface.h"
#include "UniverseInterface.h"

class DataBus;
class DataBusConnection;

class BasicJumpDriveMk1 : public ShipSystemBase {
public:
    BasicJumpDriveMk1();
    ~BasicJumpDriveMk1();
    void Init(DataBus * bus);
    void Update(double time_step);

private:
    StarCollectionType stars_;
    UniverseInterface * universe_;
    double position_x_;
    double position_y_;
    double range_;
};

#endif // BASIC_JUMP_DRIVER_MK1_H_