#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include "singleton.h"

#include <unordered_map>
#include <mutex>

// NOTE : For multi-threaded access to held objects, provide necessary lock mechanisms.
class ObjectManager : public Singleton<ObjectManager> {
public:
    explicit ObjectManager(token) {
        termination_ = false;
    }
    ~ObjectManager() {}
    void Set(std::string label, void *object) {
        std::lock_guard<std::mutex> lock(access_mutex_);
        if (termination_) return;

        auto f = objects_.find(label);
        if (f == objects_.end()) {
            objects_[label] = object;
        }
        else {
            throw std::string("Object already set!");
        }
    }
    void *Get(std::string label) {
        std::lock_guard<std::mutex> lock(access_mutex_);
        if (termination_) return 0;

        auto f = objects_.find(label);
        if (f != objects_.end()) {
            return objects_[label];
        }
        else {
            return 0;
        }
    }
    void Remove(std::string label) {
        std::lock_guard<std::mutex> lock(access_mutex_);
        termination_ = true;

        auto f = objects_.find(label);
        if (f != objects_.end()) {
            objects_.erase(label);
        }
    }
private:
    bool termination_;
    std::mutex access_mutex_;
    std::unordered_map<std::string, void *> objects_;
};

#define OBJMGR ObjectManager::Instance()

#endif // OBJECT_MANAGER_H_

