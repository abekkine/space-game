#ifndef GAME_TIMER_H_
#define GAME_TIMER_H_

#include <thread>

class GameTimer {
public:
    GameTimer() {}
    ~GameTimer() {}
    void Init() {
        t_begin_ = std::chrono::steady_clock::now();
        t_end_ = t_begin_;
    }
    double GetElapsed() {
        t_begin_ = std::chrono::steady_clock::now();
        double delta_time = std::chrono::duration<double> (t_begin_ - t_end_).count();
        t_end_ = t_begin_;
        return delta_time;
    }
    static void Sleep(int milliseconds) {

        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> t_begin_;
    std::chrono::time_point<std::chrono::steady_clock> t_end_;
};

#endif // GAME_TIMER_H_

