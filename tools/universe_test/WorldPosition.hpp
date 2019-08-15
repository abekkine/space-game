#ifndef WORLD_POSITION_HPP_
#define WORLD_POSITION_HPP_

struct WorldPosition {
    WorldPosition(double x, double y)
    : x(x)
    , y(y)
    {}
    WorldPosition() : WorldPosition(0.0, 0.0) {}
    ~WorldPosition() {}
    void Set(const double & xv, const double & yv) {
        x = xv;
        y = yv;
    }

    double x;
    double y;
};

#endif // WORLD_POSITION_HPP_
