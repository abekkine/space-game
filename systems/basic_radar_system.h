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

        if (planets_ != 0) {
            BD_RadarData radarData;
            radarData.cross_section = 1.0;
            b2Vec2 distance = planets_[0].GetDistance(plr_x_, plr_y_);
            radarData.u = distance.x / distance.Length();
            radarData.v = distance.y / distance.Length();
            DATABUS.Publish(db_RadarData, &radarData);
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

