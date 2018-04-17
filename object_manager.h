#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include "singleton.h"

#include <unordered_map>

class ObjectManager : public Singleton<ObjectManager> {
public:
    explicit ObjectManager(token) {}
    ~ObjectManager() {}
    void Set(std::string label, void *object) {
        auto f = objects_.find(label);
        if (f == objects_.end()) {
            objects_[label] = object;
        }
        else {
            throw std::string("Object already set!");
        }
    }
    void *Get(std::string label) {
        auto f = objects_.find(label);
        if (f != objects_.end()) {
            return objects_[label];
        }
        else {
            throw std::string("Object not found!");
        }
    }
private:
    std::unordered_map<std::string, void *> objects_;
};

#define OBJMGR ObjectManager::Instance()

#endif // OBJECT_MANAGER_H_

