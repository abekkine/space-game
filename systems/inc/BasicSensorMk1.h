#ifndef BASIC_SENSOR_MK1_H_
#define BASIC_SENSOR_MK1_H_

#include "ShipSystemBase.h"

class ShipModelInterface;

class BasicSensorMk1 : public ShipSystemBase {
public:
    BasicSensorMk1();
    ~BasicSensorMk1();
    void Init(DataBus * bus);
    void Update(double time_step);

private:
    ShipModelInterface * ship_model_;
};

#endif // BASIC_SENSOR_MK1_H_
