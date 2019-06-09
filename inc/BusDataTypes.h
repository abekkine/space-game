#ifndef BUS_DATA_TYPES_H_
#define BUS_DATA_TYPES_H_

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
    db_ShipDamage,
    db_ThrottleCommand,
    db_SteerCommand,
    db_StabilizeCommand,
    // Add more when needed...
    db_All = 255,
};

struct BusDataInterface {
    virtual ~BusDataInterface() {}
};

struct BD_Scalar : public BusDataInterface {
    double value;
};

struct BD_Vector : public BusDataInterface {
    double x, y;
};

struct BD_BasicPosition : public BusDataInterface {
    double x, y;
    double angle;
};

// data[ 0] -> center, data[   1] -> horizon,
// data[ 2] -> center, data[   3] -> horizon,
// ...
// data[2n] -> center, data[2n+1] -> horizon 
// num_detections = 2(n+1)
struct BD_RadarDetectionList : public BusDataInterface {
    int num_detections;
    double *data;
};


#endif // BUS_DATA_TYPES_H_
