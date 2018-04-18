#ifndef DATA_BUS_H_
#define DATA_BUS_H_

#include <functional>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "singleton.h"
struct BusDataInterface {
    virtual ~BusDataInterface() {}
};

enum DataChannel {
    db_None = 0,
    db_PlayerPosition,
    db_PlayerAngle,
    db_PlayerVelocity,
    db_PlayerThrust,
    db_PlayerGravity,
    db_PlayerFuel,
    db_DetectionList,
    // [TODO] : add more channels here when needed...
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

class DataBus : public Singleton<DataBus> {
public:
    explicit DataBus(token) {
    }
    ~DataBus() {}
    bool CheckChannel(DataChannel channel) {
        auto found = subscribers_.find(channel);
        if (found != subscribers_.end()) {
            return true;
        }
        else {
            return false;
        }
    }
    void Publish(DataChannel channel, BusDataInterface * data) {
        std::lock_guard<std::mutex> lock(bus_mutex_);
        if (CheckChannel(channel)) {
            for (auto handler : subscribers_[channel]) {
                handler(data);
            }
        }
    }
    void Subscribe(DataChannel channel, BusDataHandler handler) {
        std::lock_guard<std::mutex> lock(bus_mutex_);
        if (false == CheckChannel(channel)) {
            BusDataHandlerList v;
            subscribers_[channel] = v;
        }
        subscribers_[channel].push_back(handler);
    }

private:
    std::mutex bus_mutex_;
    std::unordered_map<DataChannel, BusDataHandlerList, std::hash<int>> subscribers_;
};

#define DATABUS DataBus::Instance()

#endif // DATA_BUS_H_
