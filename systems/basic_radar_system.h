#ifndef BASIC_RADAR_SYSTEM_H_
#define BASIC_RADAR_SYSTEM_H_

#include "ship_system_interface.h"

#include "game_data.h"
#include "data_bus.h"
#include "planet.h"

#include <math.h>

// Queries planet information from GAMEDATA and
// Sends processed detections into DATABUS.
class BasicRadarSystem : public ShipSystemInterface {
public:
    BasicRadarSystem() {}
    ~BasicRadarSystem() {}
    // From ShipSystemInterface
    void Init() {
        
        using std::placeholders::_1;
        DATABUS.Subscribe(db_PlayerPosition,
            std::bind(&BasicRadarSystem::hndPlayerPosition, this, _1));
    }
    void Update(double time_step) {
        // [TODO] : not here.
        num_planets_ = GAMEDATA.GetNumPlanets();
        planets_ = GAMEDATA.GetPlanets();
        // [END]
        (void)time_step;

        if (planets_ != 0 && num_planets_ > 0) {
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
            DATABUS.Publish(db_DetectionList, &detections);
        }
    }

private:
    void hndPlayerPosition(BusDataInterface *data) {
        BD_BasicPosition *p = static_cast<BD_BasicPosition *>(data);
        if (p != 0) {
            plr_x_ = p->x;
            plr_y_ = p->y;
        }
    }

private:
    int num_planets_;
    Planet* planets_;
    GameData::Player player_;
    double plr_x_;
    double plr_y_;
};

#endif // BASIC_RADAR_SYSTEM_H_

