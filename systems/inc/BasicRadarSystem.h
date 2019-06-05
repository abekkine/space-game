#ifndef BASIC_RADAR_SYSTEM_H_
#define BASIC_RADAR_SYSTEM_H_

#include "RadarSystemInterface.h"

class DataBus;

// Queries planet information and
// Sends processed detections into data bus.
class BasicRadarSystem : public RadarSystemInterface {
public:
    BasicRadarSystem();
    ~BasicRadarSystem();
    // From ShipSystemInterface
    void Init(DataBus * bus);
    void Update(double time_step);
};

#endif // BASIC_RADAR_SYSTEM_H_

