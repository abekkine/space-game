#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <fstream>
#include <initializer_list>

#include "json.hpp"
using json = nlohmann::json;

#include "GameException.h"
#include "Singleton.h"

class Config : public Singleton<Config> {
public:
    explicit Config(token)
    : config_file_(std::string("../config/game.json")) {
        std::fstream file(config_file_, std::fstream::in);
        file >> config_json_;
        file.close();
    }
    ~Config() {}
    json GetParam(std::initializer_list<std::string> path) {
        json object = config_json_;
        std::string name = "";
        for (auto i=path.begin(); i != path.end(); ++i) {
            name += "[" + *i + "]";
            object = object[*i];
            if (object == NULL) {
                std::string msg = "Unable to find " + name + " in " + config_file_ + "!";
                throw GameException(GameException::eParameterNotFound, msg);
            }
        }
        return object;
    }
    template <typename T>
    json GetParam(const std::initializer_list<std::string> & path, T value) {
        json param_value;
        try {
            param_value = GetParam(path);
        }
        catch (GameException & e) {
            if (e.code() == GameException::eParameterNotFound) {
                param_value = value;
            } else {
                throw;
            }
        }
        return param_value;
    }

private:
    const std::string config_file_;
    json config_json_;
};

#define CONFIG Config::Instance()

#endif  // CONFIG_H_
