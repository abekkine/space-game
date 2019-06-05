#ifndef BASIC_HULL_SYSTEM_H_
#define BASIC_HULL_SYSTEM_H_

#include "HullSystemInterface.h"

#include <functional>

class DataBus;

class BasicHullSystem : public HullSystemInterface {
private:
    double max_integrity_;
    double integrity_;
    double damage_threshold_;
    double resistance_factor_;
public:
    BasicHullSystem();
    ~BasicHullSystem();
    void Init(DataBus * bus);
    void SetDestructionCallback(std::function<void()> cb);
    void ApplyImpact(double impulse);
    double Repair(double value);
};

#endif // BASIC_HULL_SYSTEM_H_

