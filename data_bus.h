#ifndef DATA_BUS_H_
#define DATA_BUS_H_

#include <functional>
#include <unordered_map>
#include <vector>

#include "singleton.h"
struct BusDataInterface {
    virtual ~BusDataInterface() {}
};

enum DataChannel {
    db_None = 0,
    db_PlayerPosition,
    db_PlayerVelocity,
    db_PlayerThrust,
    db_PlayerGravity,
    db_PlayerFuel,
    db_Detection,
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

struct BD_RadarDetection : public BusDataInterface {
    double cross_section;
    double u, v;
};

// data[ 0] -> u1, data[   1] -> v1, data[   2] -> u2, data[   3] -> v2
// data[ 4] -> u1, data[   5] -> v1, data[   6] -> u2, data[   7] -> v2
// ...
// data[4n] -> u1, data[4n+1] -> v1, data[4n+2] -> u2, data[4n+3] -> v2
// num_detections = 4(n+1)
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
        if (CheckChannel(channel)) {
            for (auto handler : subscribers_[channel]) {
                handler(data);
            }
        }
    }
    void Subscribe(DataChannel channel, BusDataHandler handler) {
        if (false == CheckChannel(channel)) {
            BusDataHandlerList v;
            subscribers_[channel] = v;
        }
        subscribers_[channel].push_back(handler);
    }

private:
    std::unordered_map<DataChannel, BusDataHandlerList, std::hash<int>> subscribers_;
};

#define DATABUS DataBus::Instance()

#endif // DATA_BUS_H_
