#include "Config.h"
#include "GameException.h"

#include <fstream>

Config::Config(token)
: config_file_(std::string("../config/game.json")) {
    std::fstream file(config_file_, std::fstream::in);
    file >> config_json_;
    file.close();
}

Config::~Config() {}
json Config::GetParam(std::initializer_list<std::string> path) {
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
json Config::GetParam(const std::initializer_list<std::string> & path, T value) {
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

template json Config::GetParam<int>(const std::initializer_list<std::string> & path, int value);
template json Config::GetParam<double>(const std::initializer_list<std::string> & path, double value);
template json Config::GetParam<bool>(const std::initializer_list<std::string> & path, bool value);
