#ifndef SHIP_SYSTEMS_MANAGER_H_
#define SHIP_SYSTEMS_MANAGER_H_

#include "basic_engine_system.h"
#include "basic_radar_system.h"
#include "basic_hull_system.h"
#include "hotas_device.h"
#include "generic_hud_device.h"

class ShipSystemsManager : public Singleton<ShipSystemsManager> {
public:
    explicit ShipSystemsManager(token)
    : engine_(0)
    , radar_(0)
    , hull_(0)
    , hud_(0)
    , hotas_(0)
    {}
    ~ShipSystemsManager() {
    }
    void Destroy() {
        delete engine_;
        delete radar_;
        delete hud_;
        delete hull_;
        delete hotas_;
    }
    EngineSystemInterface* getEngineSystem() {
        if (engine_ == 0) {
            engine_ = new BasicEngineSystem();
        }
        return engine_;
    }
    RadarSystemInterface* getRadarSystem() {
        if (radar_ == 0) {
            radar_ = new BasicRadarSystem();
        }
        return radar_;
    }
    HullSystemInterface* getHullSystem() {
        if (hull_ == 0) {
            hull_ = new BasicHullSystem();
        }
        return hull_;
    }
    HudSystemInterface* getHudSystem() {
        if (hud_ == 0) {
            hud_ = new GenericHudDevice();
        }
        return hud_;
    }
    HotasSystemInterface* getHotasSystem() {
        if (hotas_ == 0) {
            hotas_ = new HOTASDevice();
        }
        return hotas_;
    }

private:
    EngineSystemInterface* engine_;
    RadarSystemInterface* radar_;
    HullSystemInterface* hull_;
    HudSystemInterface* hud_;
    HotasSystemInterface* hotas_;
};

#define SYSTEMSMGR ShipSystemsManager::Instance()

#endif // SHIP_SYSTEMS_MANAGER_H_
