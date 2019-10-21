#ifndef BASIC_RADAR_MK1_H_
#define BASIC_RADAR_MK1_H_

#include "ShipSystemBase.h"

#include <vector>
#include <memory>

class DataBus;
class DataBusConnection;
class Planet;

// Queries planet information and
// Sends processed detections into data bus.
class BasicRadarMk1 : public ShipSystemBase {
public:
    BasicRadarMk1();
    ~BasicRadarMk1();
    void Init(DataBus * bus);
    void Update(double time_step);
    void dbHandleShipPosition(BusDataInterface *data);

private:
    void UpdatePlanets();

private:
    std::vector<std::shared_ptr<Planet>> planets_;
    double plr_x_;
    double plr_y_;
};

#endif // BASIC_RADAR_MK1_H_
