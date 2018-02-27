#ifndef GAME_H_
#define GAME_H_

#include "display.h"
#include "input.h"

// TODO Implement game class +game +states id:2 gh:4 ic:gh
// - Add required methods to call within main.cpp
// - Add required (class) dependencies to make it work properly.
class Game {
public:
    Game()
    : input_(0)
    {}
    ~Game() {
        delete input_;
    }
    void Init(int argc, char * argv[]) {
        (void)argc;
        (void)argv;

        {
            using namespace std::placeholders;
            DISPLAY.RegisterKeyCallback(std::bind(&Input::ProcessKey, input_, _1));
        }
        DISPLAY.Init();
    }
    void Run() {

        while (! DISPLAY.QuitRequested()) {
            DISPLAY.PreRender();
            // [TODO]
            DISPLAY.PostRender();
        }

        DISPLAY.Quit();
    }
private:
    Input * input_;
};

#endif  // GAME_H_
