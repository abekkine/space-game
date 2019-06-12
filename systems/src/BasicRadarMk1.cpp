#include "BasicRadarMk1.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"
#include "Planet.h"
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

    num_planets_ = *((int *)OBJMGR.Get("nplanets"));
    assert(num_planets_ != 0 && "nplanets not defined!");

    planets_ = static_cast<Planet *>(OBJMGR.Get("planets"));
    assert(planets_ != 0 && "planets not defined");
}

void BasicRadarMk1::Update(double time_step) {
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

