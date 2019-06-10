#include "SpaceShip.h"
#include "ShipModelInterface.h"
#include "BasicShipModel.h"
#include "GameDefinitions.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "DataBusConnection.h"
#include "HotasSystemInterface.h"
#include "EngineSystemInterface.h"
#include "HudSystemInterface.h"
#include "RadarSystemInterface.h"
#include "HullSystemInterface.h"
#include "ShipSystemsManager.h"
#include "StationInterface.h"
#include "SolarSystemInterface.h"
#include "ObjectManager.h"
#include "Display.h"

#include <GLFW/glfw3.h>

#include <assert.h>

// Contact interface methods
void SpaceShip::ProcessImpulse(float impulse) {

    hull_->ApplyImpact(impulse);
}

void SpaceShip::BeginContact(ContactInterface* object) {

    if (object->Type() != 'S') return;

    StationInterface *s = static_cast<StationInterface *>(object);

    if (s != 0 && ship_anchored_ == false) {
        station_iface_ = s;
    }
}

void SpaceShip::EndContact(ContactInterface* object) {
    StationInterface *s = static_cast<StationInterface *>(object);

    if (s != 0 && ship_anchored_ == false) {
        station_iface_ = 0;
    }
}

char SpaceShip::Type() {
    return 's';
}

SpaceShip::SpaceShip()
: model_(0)
, data_bus_(0)
, bus_connection_(0)
, hud_(0)
, hotas_(0)
, engine_(0)
, radar_(0)
, hull_(0)
, landing_gear_state_(0)
, station_iface_(0)
, ship_anchored_(false)
, destroyed_(false)
{
    model_ = new BasicShipModel();

    data_bus_ = new DataBus();
    // TODO : (#148) Ship class should not have any direct connections to ship systems / devices.
    bus_connection_ = data_bus_->Connect("ship");

    hud_ = SYSTEMSMGR.getHudSystem();
    hotas_ = SYSTEMSMGR.getHotasSystem();
    engine_ = SYSTEMSMGR.getEngineSystem();
    radar_ = SYSTEMSMGR.getRadarSystem();
    hull_ = SYSTEMSMGR.getHullSystem();

    using std::placeholders::_1;
    hull_->SetDestructionCallback(std::bind(&SpaceShip::OnDestroy, this));
}

SpaceShip::~SpaceShip() {
    SYSTEMSMGR.Destroy();
    delete data_bus_;
}

void SpaceShip::SetAngle(double angle) {
    model_->SetAngle(angle);
}

double SpaceShip::GetAngle() {
    return model_->GetAngle();
}

void SpaceShip::SetPosition(double x, double y) {
    model_->SetPosition(x, y);
}

b2Vec2 const & SpaceShip::GetPosition() {
    return model_->GetPosition();
}

double SpaceShip::GetSpeed() {
    return model_->GetSpeed();
}

double SpaceShip::Mass() {
    double total_mass = 0.0;

    if (engine_ != 0) {
        // TODO :  (#136) Update engine to return fuel mass
        total_mass = engine_->FuelMass();
    }
    total_mass += model_->GetMass();

    return total_mass;
}

void SpaceShip::OnDestroy() {
    destroyed_ = true;
    hud_->Disable();
    hotas_->Disable();
}

void SpaceShip::Init(b2World * world) {
    // Init model
    model_->Init(world, this);

    // Init engine system.
    engine_->Init(data_bus_);
    engine_->Mount( model_->GetEngineMount() );

    radar_->Init(data_bus_);

    hud_->Init(data_bus_);
    hotas_->Init(data_bus_);
    hull_->Init(data_bus_);
}

void SpaceShip::Update(double delta_time) {

    model_->Update(delta_time);
    if (bus_connection_ != 0) {
        BD_Vector v_value;
        BD_Scalar s_value;

        model_->GetGravity(v_value.x, v_value.y);
        bus_connection_->Publish(db_ShipGravity, &v_value);

        // Send player velocity to Data Bus.
        // TODO : (#148) Should be published by a sensor device.
        // Used by HUD system.
        model_->GetVelocity(v_value.x, v_value.y);
        bus_connection_->Publish(db_ShipVelocity, &v_value);

        // TODO : (#148) Should be published by a sensor device.
        s_value.value = model_->GetAngularVelocity();
        bus_connection_->Publish(db_ShipAngularVelocity, &s_value);

        // TODO : (#148) Should be published by a sensor device.
        // Used by HUD & Radar systems.
        b2Vec2 pos = model_->GetPosition();
        v_value.x = pos.x;
        v_value.y = pos.y;
        bus_connection_->Publish(db_ShipPosition, &v_value);

        // TODO : (#148) Should be published by a sensor device.
        // Used by HUD system.
        s_value.value = model_->GetAngle();
        bus_connection_->Publish(db_ShipAngle, &s_value);
    }

    engine_->Update(delta_time);
    radar_->Update(delta_time);
}

void SpaceShip::Render() {
    model_->Render();

    DISPLAY.UiMode();
    hud_->Render();
    if (station_iface_ != 0 && ship_anchored_) {
        station_iface_->RenderUi();
    }
}

void SpaceShip::RefuelRequest() {
    if (station_iface_ != 0 && ship_anchored_) {
        station_iface_->Refuel(engine_);
    }
}

void SpaceShip::RepairRequest() {
    if (station_iface_ != 0 && ship_anchored_) {
        station_iface_->Repair(hull_);
    }
}

void SpaceShip::HotasInput(int key, bool action) {
    assert(hotas_ != 0);
    switch(key) {
    case GLFW_KEY_W: // main thruster
        if (action == true) {
            // enable thruster.
            hotas_->SetThrottle(1.0);
        }
        else {
            // disable thruster.
            hotas_->SetThrottle(0.0);
        }
        break;
    case GLFW_KEY_A: // right thruster
        if (action == true) {
            hotas_->SetSteering(-1.0);
        }
        else {
            hotas_->SetSteering(0.0);
        }
        break;
    case GLFW_KEY_D: // left thruster
        if (action == true) {
            hotas_->SetSteering(1.0);
        }
        else {
            hotas_->SetSteering(0.0);
        }
        break;
    case GLFW_KEY_S: // stop rotation
        if (action == true) {
            hotas_->Stabilize();
        }
        break;
    case GLFW_KEY_F:
        if (action == true) {
            RefuelRequest();
        }
        break;
    case GLFW_KEY_R:
        if (action == true) {
            RepairRequest();
        }
        break;
    case GLFW_KEY_H:
        if (action == true) {
            if (ship_anchored_ == false) {
                // Dock ship to station
                ship_anchored_ = model_->Dock(station_iface_);
            } else {
                // Undock
                ship_anchored_ = model_->Undock(station_iface_);
            }
        }
        break;
    case GLFW_KEY_G:
        // TODO : (#147) pass landing gear command to landing gear 'system' through hotas.
        // hotas_->ToggleLandingGear();
        if (action) {
            if (landing_gear_state_) {
                landing_gear_state_ = 0;
            } else {
                landing_gear_state_ = 1;
            }
            // Control Landing gear on model.
            model_->LandingGear(landing_gear_state_);
        }
        break;

#ifdef DISCONNECT_TEST
    case GLFW_KEY_1: // hud
        hud_->Disconnect();
        break;
    case GLFW_KEY_2: // ship
        data_bus_->Disconnect("ship", bus_connection_);
        break;
    case GLFW_KEY_3: // engine
        engine_->Disconnect();
        break;
    case GLFW_KEY_4: // hull
        hull_->Disconnect();
        break;
    case GLFW_KEY_5: // radar
        radar_->Disconnect();
        break;
#endif // DISCONNECT_TEST
    }
}
