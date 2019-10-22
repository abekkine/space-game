#ifndef BASIC_JUMP_DRIVER_MK1_H_
#define BASIC_JUMP_DRIVER_MK1_H_

#include "ShipSystemBase.h"
#include "UniverseInterface.h"

#include <memory>

class DataBus;
class DataBusConnection;
class StarInterface;
class SolarSystemInterface;

class BasicJumpDriveMk1 : public ShipSystemBase {
public:
    BasicJumpDriveMk1(const double & x, const double & y);
    BasicJumpDriveMk1();
    ~BasicJumpDriveMk1();
    void Init(DataBus * bus);
    void Update(double time_step);
    void dbHandleShipAngle(BusDataInterface *data);
    void dbHandleBusCommand(BusDataInterface * data);

private:
    void JumpToSystem();

private:
    StarCollectionType stars_;
    std::shared_ptr<UniverseInterface> universe_;
    std::shared_ptr<SolarSystemInterface> solar_system_;
    StarInterface * destination_star_;
    double destination_x_;
    double destination_y_;
    double position_x_;
    double position_y_;
    double range_;
    double ship_angle_;
};

#endif // BASIC_JUMP_DRIVER_MK1_H_
