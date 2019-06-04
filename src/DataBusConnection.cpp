#include "DataBusConnection.h"

#include <assert.h>

DataBusConnection::DataBusConnection(DataBus* bus) : bus_(bus) {
    assert(bus_ != 0);
}
DataBusConnection::~DataBusConnection() {
    subscribers_.clear();
}
bool DataBusConnection::CheckChannel(DataChannel channel) {
    auto found = subscribers_.find(channel);
    if (found != subscribers_.end()) {
        return true;
    }
    else {
        return false;
    }
}
void DataBusConnection::Subscribe(DataChannel channel, BusDataHandler handler) {
    if (false == CheckChannel(channel)) {
        BusDataHandlerList v;
        subscribers_[channel] = v;
    }
    subscribers_[channel].push_back(handler);
}
void DataBusConnection::Publish(DataChannel channel, BusDataInterface * data) {
    bus_->Send(channel, data);
}
