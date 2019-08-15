#ifndef SCREEN_POSITION_HPP_
#define SCREEN_POSITION_HPP_

struct ScreenPosition {
    ScreenPosition(int x, int y)
    : x(x)
    , y(y)
    {}
    ScreenPosition() : ScreenPosition(0, 0) {}
    ~ScreenPosition() {}
    void Set(int xv, int yv) {
        x = xv;
        y = yv;
    }

    int x;
    int y;
};

#endif // SCREEN_POSITION_HPP_
