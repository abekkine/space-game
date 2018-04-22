#ifndef BASIC_RADAR_SYSTEM_H_
#define BASIC_RADAR_SYSTEM_H_

#include "radar_system_interface.h"

#include <math.h>

// Queries planet information and
// Sends processed detections into data bus.
class BasicRadarSystem : public RadarSystemInterface {
public:
    BasicRadarSystem() {}
    ~BasicRadarSystem() {}
    // From ShipSystemInterface
    void Init(DataBus * bus) {

        RadarSystemInterface::Init(bus);
    }
    void Update(double time_step) {
        (void)time_step;

        if (bus_connection_ != 0) {
            if (planets_ != 0 && num_planets_ > 0) {
                // Used by HUD system.
                BD_RadarDetectionList detections;
                detections.num_detections = num_planets_;
                detections.data = new double[2*num_planets_];

                double h_angle = 0.0;
                double c_angle = 0.0;
                b2Vec2 d;
                for (int i=0; i<num_planets_; ++i) {
                    d = planets_[i].GetDistance(plr_x_, plr_y_);
                    c_angle = atan2(d.y, d.x);
                    h_angle = planets_[i].GetHorizonAngle(plr_x_, plr_y_);
                    detections.data[2*i+0] = c_angle;
                    detections.data[2*i+1] = h_angle;
                }
                bus_connection_->Publish(db_DetectionList, &detections);
            }
        }
    }
};

#endif // BASIC_RADAR_SYSTEM_H_

