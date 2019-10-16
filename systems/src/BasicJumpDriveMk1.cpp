#include "BasicJumpDriveMk1.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"
#include "ObjectManager.h"

#include <math.h>

BasicJumpDriveMk1::BasicJumpDriveMk1(const double & x, const double & y)
: position_x_(x), position_y_(y)
{
    name_ = "jumpdrive";
    range_ = 10.0;
    universe_ = 0;
}

BasicJumpDriveMk1::BasicJumpDriveMk1()
: BasicJumpDriveMk1(0.0, 0.0)
{
}

BasicJumpDriveMk1::~BasicJumpDriveMk1() {
}

void BasicJumpDriveMk1::dbHandleShipAngle(BusDataInterface *data) {
    BD_Scalar *p = static_cast<BD_Scalar *>(data);
    if (p != 0) {
        ship_angle_ = p->value;
    }
}

void BasicJumpDriveMk1::Init(DataBus * bus) {

    ShipSystemBase::Init(bus);

    if (bus_connection_ != 0) {
        DB_SUBSCRIBE(BasicJumpDriveMk1, ShipAngle);
    }

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
    detections.selected_index = -1;
    detections.data = new double[4 * detections.num_detections];

    const double aRad = ship_angle_ * M_PI / 180.0;
    const double ca = -sin(aRad);
    const double sa = cos(aRad);
    double c_angle = 0.0;
    double sx, sy;
    double dx, dy;
    float * color;
    double minAngle = 1000.0;
    for (int i=0; i<detections.num_detections; ++i) {
        stars_[i]->GetPosition(sx, sy);
        color = stars_[i]->GetColor();
        dx = sx - position_x_;
        dy = sy - position_y_;
        c_angle = atan2(dy, dx);
        double cost = (ca*dx + sa*dy) / sqrt(dx*dx + dy*dy);
        double cosAngle = fabs(cost - 1.0);
        if (cosAngle < minAngle) {
            minAngle = cosAngle;
            detections.selected_index = i;
        }
        detections.data[4 * i + 0] = c_angle;
        detections.data[4 * i + 1] = color[0];
        detections.data[4 * i + 2] = color[1];
        detections.data[4 * i + 3] = color[2];
    }
    bus_connection_->Publish(db_StarList, &detections);
}
