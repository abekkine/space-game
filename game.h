#ifndef GAME_H_
#define GAME_H_

#include "display.h"

// TODO Implement game class +game +states id:2 gh:4 ic:gh
// - Add required methods to call within main.cpp
// - Add required (class) dependencies to make it work properly.
class Game {
public:
    Game()
    : display_(0)
    {}
    ~Game() {
        delete display_;
    }
    void Init(int argc, char * argv[]) {
        (void)argc;
        (void)argv;

        display_ = new Display();
        display_->Init();
    }
    void Run() {

        while (! display_->QuitRequested()) {
            display_->PreRender();
            // [TODO]
            display_->PostRender();
        }

        display_->Quit();
    }
private:
    Display * display_;
};

#endif  // GAME_H_
