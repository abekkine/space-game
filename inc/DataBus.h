#ifndef DATA_BUS_H_
#define DATA_BUS_H_

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <mutex>

// TODO : Move definitions
struct BusDataInterface {
    virtual ~BusDataInterface() {}
};

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

typedef std::function<void(BusDataInterface *)> BusDataHandler;
typedef std::vector<BusDataHandler> BusDataHandlerList;

class DataBusConnection;

class DataBus {
public:
    DataBus();
    ~DataBus();
    DataBusConnection* Connect(std::string label);
    void Disconnect(std::string label, DataBusConnection*& conn);
    void Send(DataChannel channel, BusDataInterface * data);

private:
    // NOTE : Unless there isn't multi-threaded access to data bus, no mutex would be needed.
    std::mutex bus_mutex_;
    std::unordered_map<std::string, DataBusConnection*> connections_;
};

#define DB_SUBSCRIBE(class, channel) bus_connection_->Subscribe(db_##channel, \
    std::bind(&class::dbHandle ## channel, this, std::placeholders::_1))

#endif // DATA_BUS_H_
