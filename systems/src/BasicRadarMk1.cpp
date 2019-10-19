#include "BasicRadarMk1.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"
#include "Planet.h"
#include "SolarSystemInterface.h"
#include "ObjectManager.h"

#include <math.h>
#include <assert.h>

BasicRadarMk1::BasicRadarMk1() {
    name_ = "radar";
}

BasicRadarMk1::~BasicRadarMk1() {}

void BasicRadarMk1::dbHandleShipPosition(BusDataInterface *data) {
    BD_BasicPosition *p = static_cast<BD_BasicPosition *>(data);
    if (p != 0) {
        plr_x_ = p->x;
        plr_y_ = p->y;
    }
}

void BasicRadarMk1::Init(DataBus * bus) {

    ShipSystemBase::Init(bus);

    if (bus_connection_ != 0) {
        DB_SUBSCRIBE(BasicRadarMk1, ShipPosition);
    }

    SolarSystemInterface * s = static_cast<SolarSystemInterface *>(OBJMGR.Get("solar"));
    if (s == 0) {
        // TODO : Handle properly
        throw;
    }

    planets_ = s->GetPlanets();
}

void BasicRadarMk1::Update(double time_step) {
    (void)time_step;

    if (bus_connection_ != 0) {
        if (! planets_.empty()) {
            // Used by HUD system.
            const int numPlanets = planets_.size();
            BD_RadarDetectionList detections;
            detections.num_detections = numPlanets;
            detections.data = new double[2*numPlanets];

            double c_angle = 0.0;
            double s_angle = 0.0;
            b2Vec2 d;
            for (int i=0; i<numPlanets; ++i) {
                d = planets_[i]->GetDistance(plr_x_, plr_y_);
                c_angle = atan2(d.y, d.x);
                s_angle = planets_[i]->GetSpanAngle(plr_x_, plr_y_);
                detections.data[2*i+0] = c_angle;
                detections.data[2*i+1] = s_angle;
            }
            bus_connection_->Publish(db_DetectionList, &detections);
        }
    }
}
