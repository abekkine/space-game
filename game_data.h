#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include <mutex>

#include "singleton.h"

#include "game_definitions.h"
#include "data_bus.h"
#include "planet.h"

class GameData : public Singleton<GameData> {
public:
    struct Player {
        Player()
        : x(0.0)
        , y(0.0)
        , angle(30.0)
        , mass(1.0)
        , density(1.0)
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
        , speed(0.0)
        {}
        ~Player() {}
        double Mass() {
            return 1.4744 * density;
        }
        double x, y;
        double angle;
        double mass;
        double density;
        int n;
        double vertices[10][2];
        float c[3];
        double speed;
    };
    struct Thrust {
        Thrust() : x(0.0), y(0.0), moment(0.0) {}
        double x, y, moment;
    };

public:
    explicit GameData(token)
    : player_{}
    , planets_(0)
    {}
    ~GameData() {}
    void SetPlayer(const Player & player) {
        std::lock_guard<std::mutex> lock(player_mutex_);
        player_ = player;
    }
    void GetPlayer(Player * player) {
        std::lock_guard<std::mutex> lock(player_mutex_);
        *player = player_;
    }

    int num_planets_;
    void SetNumPlanets(int num) {
        std::lock_guard<std::mutex> lock(planet_mutex_);
        num_planets_ = num;
    }
    int GetNumPlanets() {
        std::lock_guard<std::mutex> lock(planet_mutex_);
        return num_planets_;
    }

    void SetPlanets(Planet * planets) {
        std::lock_guard<std::mutex> lock(planet_mutex_);
        planets_ = planets;
    }
    Planet* GetPlanets() {
        std::lock_guard<std::mutex> lock(planet_mutex_);
        return planets_;
    }

    void SetThrust(double m, double l, double r) {
        std::lock_guard<std::mutex> lock(thrust_mutex_);
        thrust_.x = m * cos(0.5 * M_PI + (player_.angle * M_PI / 180.0));
        thrust_.y = m * sin(0.5 * M_PI + (player_.angle * M_PI / 180.0));
        thrust_.moment = r - l;

        BD_Vector player_thrust;
        player_thrust.x = thrust_.x;
        player_thrust.y = thrust_.y;
        DATABUS.Publish(db_PlayerThrust, &player_thrust);
    }
    void GetThrust(double & thrust_x, double & thrust_y) {
        std::lock_guard<std::mutex> lock(thrust_mutex_);
        thrust_x = thrust_.x;
        thrust_y = thrust_.y;
    }
    double GetMoment() {
        std::lock_guard<std::mutex> lock(thrust_mutex_);
        return thrust_.moment;
    }

    double gx, gy;
    std::mutex g_mutex;
    void SetGravityDebug(double x, double y) {
        std::lock_guard<std::mutex> lock(g_mutex);
        gx = x;
        gy = y;
    }
    void GetGravityDebug(double & x, double & y) {
        std::lock_guard<std::mutex> lock(g_mutex);
        x = gx;
        y = gy;
    }
    double vx, vy;
    std::mutex v_mutex;
    void SetVelocityDebug(double x, double y) {
        std::lock_guard<std::mutex> lock(v_mutex);
        vx = x;
        vy = y;
    }
    void GetVelocityDebug(double & x, double & y) {
        std::lock_guard<std::mutex> lock(v_mutex);
        x = vx;
        y = vy;
    }

    void SetState(GameDefinitions::GameStateEnum state) {
        std::lock_guard<std::mutex> lock(state_mutex_);
        game_state_ = state;
    }
    GameDefinitions::GameStateEnum GetState() {
        std::lock_guard<std::mutex> lock(state_mutex_);
        return game_state_;
    }

private:
    std::mutex player_mutex_;
    std::mutex planet_mutex_;
    std::mutex thrust_mutex_;
    std::mutex state_mutex_;

    Player player_;
    Planet* planets_;
    Thrust thrust_;

    GameDefinitions::GameStateEnum game_state_;
};

#define GAMEDATA GameData::Instance()

#endif  // GAME_DATA_H_
