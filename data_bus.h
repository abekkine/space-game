#ifndef DATA_BUS_H_
#define DATA_BUS_H_

#include <functional>
#include <unordered_map>
#include <vector>
#include <mutex>

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

class DataBus {
public:
    struct Connection {
        DataBus* bus_;
        Connection(DataBus* bus) : bus_(bus) {
            assert(bus_ != 0);
        }
        ~Connection() {
            subscribers_.clear();
        }
        bool CheckChannel(DataChannel channel) {
            auto found = subscribers_.find(channel);
            if (found != subscribers_.end()) {
                return true;
            }
            else {
                return false;
            }
        }
        void Subscribe(DataChannel channel, BusDataHandler handler) {
            if (false == CheckChannel(channel)) {
                BusDataHandlerList v;
                subscribers_[channel] = v;
            }
            subscribers_[channel].push_back(handler);
        }
        void Publish(DataChannel channel, BusDataInterface * data) {
            bus_->Send(channel, data);
        }
        std::unordered_map<DataChannel, BusDataHandlerList, std::hash<int>> subscribers_;
    };
public:
    DataBus() {}
    ~DataBus() {
        for (auto c=connections_.begin(); c!=connections_.end(); ++c) {
            delete c->second;
        }
    }
    Connection* Connect(std::string label) {
        std::lock_guard<std::mutex> lock(bus_mutex_);
        auto f = connections_.find(label);
        if (f == connections_.end()) {
            Connection *newConnection;
            newConnection = new Connection(this);
            connections_[label] = newConnection;
        }
        return connections_[label];
    }
    void Disconnect(std::string label, Connection*& conn) {
        std::lock_guard<std::mutex> lock(bus_mutex_);
        auto f= connections_.find(label);
        if (f != connections_.end()) {
            assert(conn == connections_[label]);
            connections_.erase(label);
            delete conn;
            conn = 0;
        }
    }
    void Send(DataChannel channel, BusDataInterface * data) {
        std::lock_guard<std::mutex> lock(bus_mutex_);
        for (auto c : connections_) {
            if (c.second->CheckChannel(channel)) {
                for (auto handler : c.second->subscribers_[channel]) {
                    handler(data);
                }
            }
        }
    }

private:
    // TODO : Unless there isn't multi-threaded access to
    //      : data bus, no mutex would be needed.
    std::mutex bus_mutex_;
    std::unordered_map<std::string, Connection*> connections_;
};

#define DB_SUBSCRIBE(class, channel) bus_connection_->Subscribe(db_##channel, \
    std::bind(&class::dbHandle ## channel, this, std::placeholders::_1))

#endif // DATA_BUS_H_
