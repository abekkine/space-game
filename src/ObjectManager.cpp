#include "ObjectManager.h"

ObjectManager::ObjectManager(token) {
    termination_ = false;
}
ObjectManager::~ObjectManager() {}
void ObjectManager::Set(std::string label, void *object) {
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
void *ObjectManager::Get(std::string label) {
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
void ObjectManager::Remove(std::string label) {
    std::lock_guard<std::mutex> lock(access_mutex_);
    termination_ = true;

    auto f = objects_.find(label);
    if (f != objects_.end()) {
        objects_.erase(label);
    }
}
