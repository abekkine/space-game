#include "DataBus.h"
#include "DataBusConnection.h"

#include <functional>
#include <unordered_map>
#include <vector>
#include <mutex>

#include <assert.h>

DataBus::DataBus() {}
DataBus::~DataBus() {
    for (auto c=connections_.begin(); c!=connections_.end(); ++c) {
        delete c->second;
    }
}
DataBusConnection* DataBus::Connect(std::string label) {
    std::lock_guard<std::mutex> lock(bus_mutex_);
    auto f = connections_.find(label);
    if (f == connections_.end()) {
        DataBusConnection *newConnection;
        newConnection = new DataBusConnection(this);
        connections_[label] = newConnection;
    }
    return connections_[label];
}
void DataBus::Disconnect(std::string label, DataBusConnection*& conn) {
    std::lock_guard<std::mutex> lock(bus_mutex_);
    auto f= connections_.find(label);
    if (f != connections_.end()) {
        assert(conn == connections_[label]);
        connections_.erase(label);
        delete conn;
        conn = 0;
    }
}
void DataBus::Send(DataChannel channel, BusDataInterface * data) {
    std::lock_guard<std::mutex> lock(bus_mutex_);
    for (auto c : connections_) {
        if (c.second->CheckChannel(channel)) {
            for (auto handler : c.second->subscribers_[channel]) {
                handler(data);
            }
        }
    }
}

