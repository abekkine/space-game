#ifndef SHIP_SYSTEMS_MANAGER_H_
#define SHIP_SYSTEMS_MANAGER_H_

#include "Singleton.hpp"
#include "BasicEngineMk1.h"
#include "BasicRadarMk1.h"
#include "BasicJumpDriveMk1.h"
#include "BasicHullSystem.h"
#include "HotasDevice.h"
#include "GenericHudDevice.h"
#include "BasicSensorMk1.h"

class ShipSystemsManager : public Singleton<ShipSystemsManager> {
public:
    explicit ShipSystemsManager(token)
    : engine_(0)
    , radar_(0)
    , jump_drive_(0)
    , hull_(0)
    , hud_(0)
    , hotas_(0)
    , sensor_(0)
    {}
    ~ShipSystemsManager() {
    }
    void Destroy() {
        delete engine_;
        delete radar_;
        delete jump_drive_;
        delete hud_;
        delete hull_;
        delete hotas_;
        delete sensor_;
    }
    EngineSystemInterface* getEngineSystem() {
        if (engine_ == 0) {
            engine_ = new BasicEngineMk1();
        }
        return engine_;
    }
    ShipSystemBase* getRadarSystem() {
        if (radar_ == 0) {
            radar_ = new BasicRadarMk1();
        }
        return radar_;
    }
    ShipSystemBase* getJumpDriveSystem(const double & map_x, const double & map_y) {
        if (jump_drive_ == 0) {
            jump_drive_ = new BasicJumpDriveMk1(map_x, map_y);
        }

        return jump_drive_;
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
    ShipSystemBase * getSensorSystem() {
        if (sensor_ == 0) {
            sensor_ = new BasicSensorMk1();
        }
        return sensor_;
    }

private:
    EngineSystemInterface* engine_;
    ShipSystemBase* radar_;
    ShipSystemBase* jump_drive_;
    HullSystemInterface* hull_;
    HudSystemInterface* hud_;
    HotasSystemInterface* hotas_;
    ShipSystemBase * sensor_;
};

#define SYSTEMSMGR ShipSystemsManager::Instance()

#endif // SHIP_SYSTEMS_MANAGER_H_
