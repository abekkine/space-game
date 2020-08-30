#ifndef BACKGROUND_H_
#define BACKGROUND_H_

class Background {
private:
    const double kSpan;
    const double kStep;
    const double kFarFactor;
    const int kNumNearPoints;
    const int kNumFarPoints;
    double near_points_[1000];
    double far_points_[1000];
public:
    Background();
    ~Background();
    void Init();
    void Render(double x, double y, double angle);
};

#endif // BACKGROUND_H_
