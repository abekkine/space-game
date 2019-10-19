#include "BasicJumpDriveMk1.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"
#include "ObjectManager.h"

#include <math.h>

BasicJumpDriveMk1::BasicJumpDriveMk1() {
    name_ = "jumpdrive";
    // Some selected star
    position_x_ = -10.6018;
    position_y_ = 2.9680;
    range_ = 10.0;
    universe_ = 0;
}

BasicJumpDriveMk1::~BasicJumpDriveMk1() {
}

void BasicJumpDriveMk1::Init(DataBus * bus) {

    ShipSystemBase::Init(bus);

    universe_ = std::static_pointer_cast<UniverseInterface>(OBJMGR.Get("universe"));
}

void BasicJumpDriveMk1::Update(double time_step) {
    (void)time_step;
    if (bus_connection_ == 0) {
        return;
    }

    if (universe_ == 0) {
        throw std::string("Universe not defined!");
    }
    
    universe_->GetStars(position_x_, position_y_, range_, stars_);

    BD_StarDetectionList detections;
    detections.num_detections = stars_.size();
    detections.data = new double[4 * detections.num_detections];

    double c_angle = 0.0;
    double sx, sy;
    double dx, dy;
    float * color;
    for (int i=0; i<detections.num_detections; ++i) {
        stars_[i]->GetPosition(sx, sy);
        color = stars_[i]->GetColor();
        dx = sx - position_x_;
        dy = sy - position_y_;
        c_angle = atan2(dy, dx);
        detections.data[4 * i + 0] = c_angle;
        detections.data[4 * i + 1] = color[0];
        detections.data[4 * i + 2] = color[1];
        detections.data[4 * i + 3] = color[2];
    }
    bus_connection_->Publish(db_StarList, &detections);
}