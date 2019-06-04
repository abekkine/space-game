#ifndef RADAR_SYSTEM_INTERFACE_H_
#define RADAR_SYSTEM_INTERFACE_H_

#include "ShipSystemInterface.h"

#include "Planet.h"
#include "ObjectManager.h"

#include <assert.h>

class RadarSystemInterface : public ShipSystemInterface {
public:
    virtual ~RadarSystemInterface() {}
public:
// Standard ship system interface
    virtual void Init(DataBus * bus) {
        assert(bus != 0);
        bus_ = bus;

        bus_connection_ = bus_->Connect("radar");
        if (bus_connection_ != 0) {
            DB_SUBSCRIBE(RadarSystemInterface, ShipPosition);
        }

        num_planets_ = *((int *)OBJMGR.Get("nplanets"));
        assert(num_planets_ != 0 && "nplanets not defined!");

        planets_ = static_cast<Planet *>(OBJMGR.Get("planets"));
        assert(planets_ != 0 && "planets not defined");
    }
    virtual void Update(double time_step) { (void)time_step; }
    virtual void Disconnect() {
        bus_->Disconnect("radar", bus_connection_);
    }

private:
    void dbHandleShipPosition(BusDataInterface *data) {
        BD_BasicPosition *p = static_cast<BD_BasicPosition *>(data);
        if (p != 0) {
            plr_x_ = p->x;
            plr_y_ = p->y;
        }
    }

protected:
    int num_planets_;
    Planet* planets_;
    double plr_x_;
    double plr_y_;
};

#endif // RADAR_SYSTEM_INTERFACE_H_

