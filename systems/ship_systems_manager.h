#ifndef SHIP_SYSTEMS_MANAGER_H_
#define SHIP_SYSTEMS_MANAGER_H_

#include "engine_system_interface.h"
#include "basic_engine_system.h"

class ShipSystemsManager : public Singleton<ShipSystemsManager> {
public:
    explicit ShipSystemsManager(token)
    : ship_engine_(0)
    {}
    ~ShipSystemsManager() {}
    void createEngineSystem() {
        // Decide which engine to create by ship configuration
        ship_engine_ = new BasicEngineSystem();
    }
    EngineSystemInterface* getEngineSystem() {
        return ship_engine_;
    }

private:
    EngineSystemInterface* ship_engine_;
};

#define SYSTEMSMGR ShipSystemsManager::Instance()

#endif // SHIP_SYSTEMS_MANAGER_H_
