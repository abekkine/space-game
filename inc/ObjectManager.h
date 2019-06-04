#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include "Singleton.h"

#include <string>
#include <unordered_map>
#include <mutex>

// NOTE : For multi-threaded access to held objects, provide necessary lock mechanisms.
class ObjectManager : public Singleton<ObjectManager> {
public:
    explicit ObjectManager(token);
    ~ObjectManager();
    void Set(std::string label, void *object);
    void *Get(std::string label);
    void Remove(std::string label);

private:
    bool termination_;
    std::mutex access_mutex_;
    std::unordered_map<std::string, void *> objects_;
};

#define OBJMGR ObjectManager::Instance()

#endif // OBJECT_MANAGER_H_

