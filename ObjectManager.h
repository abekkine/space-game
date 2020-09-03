#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include "Singleton.hpp"

#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

class StorableObject;

// NOTE : For multi-threaded access to held objects, provide necessary lock mechanisms.
class ObjectManager : public Singleton<ObjectManager> {
public:
    explicit ObjectManager(token);
    ~ObjectManager();
    void Set(std::string label, std::shared_ptr<StorableObject> object, bool replace=false);
    std::shared_ptr<StorableObject> Get(std::string label);
    void Remove(std::string label);

private:
    bool termination_;
    std::mutex access_mutex_;
    std::unordered_map<std::string, std::shared_ptr<StorableObject>> objects_;
};

#define OBJMGR ObjectManager::Instance()

#endif // OBJECT_MANAGER_H_
