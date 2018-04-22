#ifndef SHIP_SYSTEMS_MANAGER_H_
#define SHIP_SYSTEMS_MANAGER_H_

#include "basic_engine_system.h"
#include "basic_radar_system.h"
#include "basic_hull_system.h"

class ShipSystemsManager : public Singleton<ShipSystemsManager> {
public:
    explicit ShipSystemsManager(token)
    : engine_(0)
    , radar_(0)
    , hull_(0)
    {}
    ~ShipSystemsManager() {}
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
private:
    EngineSystemInterface* engine_;
    RadarSystemInterface* radar_;
    HullSystemInterface* hull_;
};

#define SYSTEMSMGR ShipSystemsManager::Instance()

#endif // SHIP_SYSTEMS_MANAGER_H_
