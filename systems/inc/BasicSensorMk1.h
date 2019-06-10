#ifndef BASIC_SENSOR_MK1_H_
#define BASIC_SENSOR_MK1_H_

#include "ShipSystemInterface.h"

class ShipModelInterface;

class BasicSensorMk1 : public ShipSystemInterface {
public:
    BasicSensorMk1();
    ~BasicSensorMk1();
    void Init(DataBus * bus);
    void Update(double time_step);
    void Disconnect();

private:
    ShipModelInterface * ship_model_;
};

#endif // BASIC_SENSOR_MK1_H_
