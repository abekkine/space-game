#include "BasicSensorMk1.h"
#include "ShipModelInterface.h"
#include "ObjectManager.h"

#include <assert.h>

BasicSensorMk1::BasicSensorMk1() {
}

BasicSensorMk1::~BasicSensorMk1() {
}

void BasicSensorMk1::Init(DataBus * bus) {
    assert(bus != 0);
    bus_ = bus;
    bus_connection_ = bus_->Connect("sensor");
    ship_model_ = static_cast<ShipModelInterface *>(OBJMGR.Get("model"));
}

void BasicSensorMk1::Update(double time_step) {
    (void)time_step;
    if (bus_connection_ == 0 || ship_model_ == 0) {
        return;
    }

    BD_Vector v_value;
    BD_Scalar s_value;

    ship_model_->GetGravity(v_value.x, v_value.y);
    bus_connection_->Publish(db_ShipGravity, &v_value);

    // Send player velocity to Data Bus.
    // Used by HUD system.
    ship_model_->GetVelocity(v_value.x, v_value.y);
    bus_connection_->Publish(db_ShipVelocity, &v_value);

    s_value.value = ship_model_->GetAngularVelocity();
    bus_connection_->Publish(db_ShipAngularVelocity, &s_value);

    // Used by HUD & Radar systems.
    b2Vec2 pos = ship_model_->GetPosition();
    v_value.x = pos.x;
    v_value.y = pos.y;
    bus_connection_->Publish(db_ShipPosition, &v_value);

    // Used by HUD system.
    s_value.value = ship_model_->GetAngle();
    bus_connection_->Publish(db_ShipAngle, &s_value);
}

void BasicSensorMk1::Disconnect() {
    bus_->Disconnect("sensor", bus_connection_);
}
