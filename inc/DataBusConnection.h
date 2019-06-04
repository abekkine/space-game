#ifndef DATA_BUS_CONNECTION_H_
#define DATA_BUS_CONNECTION_H_

#include "DataBus.h"

#include <unordered_map>

struct DataBusConnection {

    explicit DataBusConnection(DataBus* bus);
    ~DataBusConnection();
    bool CheckChannel(DataChannel channel);
    void Subscribe(DataChannel channel, BusDataHandler handler);
    void Publish(DataChannel channel, BusDataInterface * data);

    DataBus* bus_;
    std::unordered_map<DataChannel, BusDataHandlerList, std::hash<int>> subscribers_;
};

#endif // DATA_BUS_CONNECTION_H_