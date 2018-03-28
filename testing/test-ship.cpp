#include <unistd.h>

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex_;
void print_function(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << message << '\n';
}

struct SharedData {

    SharedData() {
        for (int i=0; i<10; ++i) {
            data[i].x = 1.0;
            data[i].y = 2.0;
            data[i].angle = 30.0;
            data[i].mass = 1.0;
        }
    }

    struct {
        double x;
        double y;
        double angle;
        double mass;
    } data[10];

    void ReadData() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (int i=0; i<10; ++i) {
            std::cout << i << ' ' << data[i].x << ' ' << data[i].y << ' ' << data[i].angle << ' ' << data[i].mass << '\n';
        }
    }
    void UpdateData() {
        std::lock_guard<std::mutex> lock(mutex_);

        double op[2] = { 0.01, -0.01 };
        int ix = rand() % 10;
        int n = rand() % 4;
        int opix = rand() % 2;

        switch (n) {
            case 0:
                data[ix].x += op[opix]; break;
            case 1:
                data[ix].y += op[opix]; break;
            case 2:
                data[ix].angle += op[opix]; break;
            case 3:
                data[ix].mass += op[opix]; break;
        }
    }
    void SetMass(int i, double value) {
        std::lock_guard<std::mutex> lock(mutex_);

        std::cout << "SetMass(" << i << ", " << value << ")\n";
        data[i].mass = value;
    }
    double GetMass(int i) {
        std::lock_guard<std::mutex> lock(mutex_);

        return data[i].mass;
    }

private:
    std::mutex mutex_;
};

class Display {
public:
    Display(SharedData* data)
    : counter_(10000)
    , data_(data)
    {
        std::cout << "Display::Display()\n";
    }
    ~Display() {
        std::cout << "Display::~Display()\n";
        thread_.join();
    }
    void Start() {
        std::cout << "Display::Start() -- create thread.\n";
        thread_ = std::thread(&Display::ThreadLoop, this);
    }
    void ThreadLoop() {
        std::cout << "Display::ThreadLoop()\n";
        while (counter_ > 0) {
            sleep(1);
            data_->ReadData();
            --counter_;
        }
    }
private:
    int counter_;
    std::thread thread_;
    SharedData * data_;
};

class Physics {
public:
    Physics(SharedData * data)
    : counter_(10000)
    , data_(data)
    {
        std::cout << "Physics::Physics()\n";
    }
    ~Physics() {
        std::cout << "Physics::~Physics()\n";
        thread_.join();
    }
    void Start() {
        std::cout << "Physics::Start() -- create thread.\n";
        thread_ = std::thread(&Physics::ThreadLoop, this);
    }
    void ThreadLoop() {
        std::cout << "Physics::ThreadLoop()\n";
        while (counter_ > 0) {
            usleep(100000);
            data_->UpdateData();
            --counter_;
        }
    }
private:
    int counter_;
    std::thread thread_;
    SharedData * data_;
};

class Game {
public:
    Game(SharedData * data)
    : counter_(10000)
    , data_(data)
    {
        std::cout << "Game::Game()\n";
    }
    ~Game() {
        std::cout << "Game::~Game()\n";
        thread_.join();
    }
    void Start() {
        std::cout << "Game::Start() -- create thread.\n";
        thread_ = std::thread(&Game::ThreadLoop, this);
    }
    void ThreadLoop() {
        std::cout << "Game::ThreadLoop()\n";
        while (counter_ > 0) {
            sleep(5);
            int k = rand() % 10;
            double val = drand48() * 10.0;
            data_->SetMass(k, val);
            --counter_;
        }
    }
private:
    int counter_;
    std::thread thread_;
    SharedData * data_;
};

int main() {

    SharedData someData;

    Display * d = new Display(&someData);
    Physics * p = new Physics(&someData);
    Game * g = new Game(&someData);

    d->Start();
    p->Start();
    g->Start();

    delete g;
    delete p;
    delete d;

    return 0;
}
