#ifndef SHIP_SYSTEMS_MANAGER_H_
#define SHIP_SYSTEMS_MANAGER_H_

#include "engine_system_interface.h"
#include "basic_engine_system.h"
#include "basic_radar_system.h"

class ShipSystemsManager : public Singleton<ShipSystemsManager> {
public:
    explicit ShipSystemsManager(token)
    : ship_engine_(0)
    , radar_(0)
    {}
    ~ShipSystemsManager() {}
    EngineSystemInterface* getEngineSystem() {
        if (ship_engine_ == 0) {
            createEngineSystem();
        }
        return ship_engine_;
    }
    BasicRadarSystem* getRadarSystem() {
        if (radar_ == 0) {
            createRadarSystem();
        }
        return radar_;
    }
private:
    void createEngineSystem() {
        // Decide which engine to create by ship configuration
        ship_engine_ = new BasicEngineSystem();
    }
    void createRadarSystem() {
        radar_ = new BasicRadarSystem();
    }
private:
    EngineSystemInterface* ship_engine_;
    BasicRadarSystem * radar_;
};

#define SYSTEMSMGR ShipSystemsManager::Instance()

#endif // SHIP_SYSTEMS_MANAGER_H_
