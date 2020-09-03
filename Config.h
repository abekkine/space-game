#ifndef CONFIG_H_
#define CONFIG_H_

#include "Singleton.hpp"

#include "json.hpp"
using json = nlohmann::json;

#include <string>
#include <initializer_list>

class Config : public Singleton<Config> {
public:
    explicit Config(token);
    ~Config();
    json GetParam(std::initializer_list<std::string> path);
    template <typename T>
    json GetParam(const std::initializer_list<std::string> & path, T value);

private:
    const std::string config_file_;
    json config_json_;
};

#define CONFIG Config::Instance()

#endif  // CONFIG_H_
