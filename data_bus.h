#ifndef DATA_BUS_H_
#define DATA_BUS_H_

#include "singleton.h"

#include <functional>
#include <unordered_map>
#include <vector>

struct BusDataInterface {
    virtual ~BusDataInterface() {}
};

enum class DataChannel : uint8_t {
    eNONE,
    // [TODO] : add here when needed...
    eALL,
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
    std::unordered_map<DataChannel, BusDataHandlerList> subscribers_;
};

#define DATABUS DataBus::Instance()

#endif // DATA_BUS_H_
