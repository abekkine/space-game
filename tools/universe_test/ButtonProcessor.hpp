#ifndef BUTTON_PROCESSOR_HPP_
#define BUTTON_PROCESSOR_HPP_

#include <functional>

enum ButtonIdType {
    btn_NONE = -1,
    btn_LEFT = 0,
    btn_MIDDLE = 1,
    btn_RIGHT = 2,
    btn_SCROLL_UP = 3,
    btn_SCROLL_DOWN = 4,
};

enum StateType {
    state_NONE = -1,
    state_PRESSED = 0,
    state_RELEASED = 1,
};

class ButtonProcessor {
public:
    ButtonProcessor()
    : button_pressed_handler_(0)
    , button_released_handler_(0)
    {}
    ~ButtonProcessor() {}
    void RegisterHandlers(
        std::function<void()> pressed_handler,
        std::function<void()> released_handler
    ) {
        button_pressed_handler_ = pressed_handler;
        button_released_handler_ = released_handler;
    }
    void Process(StateType state) {
        switch(state) {
        case state_PRESSED:
            if (button_pressed_handler_) {
                button_pressed_handler_();
            }
            break;
        case state_RELEASED:
            if (button_released_handler_) {
                button_released_handler_();
            }
            break;
        case state_NONE:
        default:
            break;
        }
    }

private:
    std::function<void()> button_pressed_handler_;
    std::function<void()> button_released_handler_;
};

#endif // BUTTON_PROCESSOR_HPP_
