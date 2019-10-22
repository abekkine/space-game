#ifndef BUS_DATA_TYPES_H_
#define BUS_DATA_TYPES_H_

#include <cstdint>

enum DataChannel {
    db_None = 0,
    db_ShipPosition,
    db_ShipAngle,
    db_ShipVelocity,
    db_ShipAngularVelocity,
    db_ShipThrust,
    db_ShipGravity,
    db_ShipFuelQty,
    db_DetectionList,
    db_StarList,
    db_ShipDamage,
    db_ThrottleCommand,
    db_SteerCommand,
    db_StabilizeCommand,
    db_BusCommand,
    // Add more when needed...
    db_All = 255,
};

struct BusDataInterface {
    virtual ~BusDataInterface() {}
};

struct BD_BusCommand : public BusDataInterface {
    uint32_t id;
    uint32_t param;
};

struct BD_Scalar : public BusDataInterface {
    double value;
};

struct BD_Vector : public BusDataInterface {
    double x, y;
};

struct BD_BasicPosition : public BusDataInterface {
    double x, y;
};

// data[ 0] -> center, data[   1] -> span,
// data[ 2] -> center, data[   3] -> span,
// ...
// data[2n] -> center, data[2n+1] -> span
// num_detections = 2(n+1)
struct BD_RadarDetectionList : public BusDataInterface {
    int num_detections;
    double *data;
};

// data[ 4 * i + 0   ] -> center
// data[ 4 * i + 1-3 ] -> colors r, g, b
struct BD_StarDetectionList : public BusDataInterface {
    int num_detections;
    int selected_index;
    double *data;
};

#endif // BUS_DATA_TYPES_H_
