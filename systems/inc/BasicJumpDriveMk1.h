#ifndef BASIC_JUMP_DRIVER_MK1_H_
#define BASIC_JUMP_DRIVER_MK1_H_

#include "ShipSystemBase.h"
#include "StarInterface.h"
#include "UniverseInterface.h"

#include <memory>

class DataBus;
class DataBusConnection;

class BasicJumpDriveMk1 : public ShipSystemBase {
public:
    BasicJumpDriveMk1(const double & x, const double & y);
    BasicJumpDriveMk1();
    ~BasicJumpDriveMk1();
    void Init(DataBus * bus);
    void Update(double time_step);
    void dbHandleShipAngle(BusDataInterface *data);

private:
    StarCollectionType stars_;
    std::shared_ptr<UniverseInterface> universe_;
    double position_x_;
    double position_y_;
    double range_;
    double ship_angle_;
};

#endif // BASIC_JUMP_DRIVER_MK1_H_
