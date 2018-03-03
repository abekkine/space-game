#ifndef INPUT_H_
#define INPUT_H_

#include <iostream>

#include "display.h"

// TODO Implement Input class +input +ui +player +menu id:1 gh:3 ic:gh
// - Handles input events.
class Input {
 public:
    Input() {}
    ~Input() {}
    void ProcessKey(int key) {
        switch (key) {
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                DISPLAY.RequestQuit();
                break;
            default:
                std::cout << key << '\n';
        }
    }
};

#endif  // INPUT_H_
