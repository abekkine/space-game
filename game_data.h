#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include <mutex>

#include "singleton.h"

class GameData : public Singleton<GameData> {
public:
    struct Player {
        Player()
        : x(0.0)
        , y(0.0)
        , a(30.0)
        , n(8)
        , vertices{ {0.16, 1.2},
                    {-0.16, 1.2},
                    {-0.5, 0.16},
                    {-0.68, 0.14},
                    {-0.54, -0.46},
                    {0.54, -0.46},
                    {0.68, 0.14},
                    {0.5, 0.16} }
        , c{1.0, 1.0, 1.0}
        {}
        double x, y;
        double a;
        int n;
        double vertices[10][2];
        float c[3];
    };
    struct Planet {
        Planet()
        : x(0.0)
        , y(0.0)
        , a(0.0)
        , r(50.0)
        , c{1.0, 0.0, 0.0}
        {}
        double x, y;
        double a;
        double r;
        float c[3];
    };
    struct Thrust {
        Thrust() : main(0.0), left(0.0), right(0.0) {}
        double main;
        double left;
        double right;
    };


public:
    explicit GameData(token)
    : player_{}
    , planet_{}
    {}
    ~GameData() {}
    void SetPlayer(const Player & player) {
        std::lock_guard<std::mutex> lock(player_mutex_);
        player_ = player;
    }
    void SetPlanet(const Planet & planet) {
        std::lock_guard<std::mutex> lock(planet_mutex_);
        planet_ = planet;
    }
    void GetPlayer(Player * player) {
        std::lock_guard<std::mutex> lock(player_mutex_);
        *player = player_;
    }
    void GetPlanet(Planet * planet) {
        std::lock_guard<std::mutex> lock(planet_mutex_);
        *planet = planet_;
    }
    void SetThrust(double m, double l, double r) {
        std::lock_guard<std::mutex> lock(thrust_mutex_);
        thrust_.main = m;
        thrust_.left = l;
        thrust_.right = r;
    }
    void GetThrust(double & thrust_x, double & thrust_y) {
        std::lock_guard<std::mutex> lock(thrust_mutex_);
        thrust_x = thrust_.main * cos(0.5 * M_PI + (player_.a * M_PI / 180.0));
        thrust_y = thrust_.main * sin(0.5 * M_PI + (player_.a * M_PI / 180.0));
    }
    double GetMoment() {
        std::lock_guard<std::mutex> lock(thrust_mutex_);
        return (thrust_.right - thrust_.left);
    }

private:
    std::mutex player_mutex_;
    std::mutex planet_mutex_;
    std::mutex thrust_mutex_;

    Player player_;
    Planet planet_;
    Thrust thrust_;
};

#define GAMEDATA GameData::Instance()

#endif  // GAME_DATA_H_
