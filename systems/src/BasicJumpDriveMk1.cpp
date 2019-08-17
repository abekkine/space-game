#include "BasicJumpDriveMk1.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"
#include "ObjectManager.h"
#include "StarCollection.hpp"

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

    universe_ = static_cast<UniverseInterface *>(OBJMGR.Get("universe"));
    stars_ = universe_->GetStars();
}

void BasicJumpDriveMk1::DetectionUpdate(std::shared_ptr<StarInterface> s) {
    double c_angle = 0.0;
    double sx, sy;
    double dx, dy;
    float * color;
    s->GetPosition(sx, sy);
    color = s->GetColor();
    dx = sx - position_x_;
    dy = sy - position_y_;
    c_angle = atan2(dy, dx);
    detections_.data[4 * detection_index_ + 0] = c_angle;
    detections_.data[4 * detection_index_ + 1] = color[0];
    detections_.data[4 * detection_index_ + 2] = color[1];
    detections_.data[4 * detection_index_ + 3] = color[2];
    ++detection_index_;
}

void BasicJumpDriveMk1::Update(double time_step) {
    (void)time_step;
    if (bus_connection_ == 0) {
        return;
    }

    if (universe_ == 0) {
        throw std::string("Universe not defined!");
    }

    universe_->UpdateStars(position_x_, position_y_, range_);
    detections_.num_detections = stars_->GetSize();
    detections_.data = new double[4 * detections_.num_detections];
    detection_index_ = 0;
    stars_->RunOperation(
        std::bind(&BasicJumpDriveMk1::DetectionUpdate, this, std::placeholders::_1)
    );
    bus_connection_->Publish(db_StarList, &detections_);
}