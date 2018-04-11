#ifndef BASIC_RADAR_SYSTEM_H_
#define BASIC_RADAR_SYSTEM_H_

#include "ship_system_interface.h"

#include "game_data.h"
#include "data_bus.h"
#include "planet.h"

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
            detections.data = new double[4*num_planets_];

            b2Vec2 horizon;
            for (int i=0; i<num_planets_; ++i) {
                horizon = planets_[i].GetHorizonVector(1.0, plr_x_, plr_y_);
                detections.data[4*i+0] = horizon.x;
                detections.data[4*i+1] = horizon.y;
                horizon = planets_[i].GetHorizonVector(-1.0, plr_x_, plr_y_);
                detections.data[4*i+2] = horizon.x;
                detections.data[4*i+3] = horizon.y;
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

