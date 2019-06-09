#ifndef DATA_BUS_H_
#define DATA_BUS_H_

#include "BusDataTypes.h"

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <mutex>

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
