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

private:
    std::mutex player_mutex_;
    std::mutex planet_mutex_;

    Player player_;
    Planet planet_;
};

#define GAMEDATA GameData::Instance()

#endif  // GAME_DATA_H_
