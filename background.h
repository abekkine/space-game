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
    Background()
    : kSpan(100.0)
    , kStep(10.0)
    , kFarFactor(0.2)
    , kNumNearPoints(600)
    , kNumFarPoints(1000)
    , near_points_{}
    , far_points_{}
    {}
    ~Background() {
    }
    void Init() {

        srand48(0x994839);
        for (int i=0; i<kNumNearPoints; ++i) {
            near_points_[i] = drand48() * kSpan;
        }
        srand48(0x99403895);
        for (int i=0; i<kNumFarPoints; ++i) {
            near_points_[i] = drand48() * kSpan;
        }
    }
    void Render(double x, double y, double angle) {

        if (near_points_ == 0 || far_points_ == 0) return;

        // Near layer.
        double region_x_base = kSpan * floor(x / kSpan);
        double region_y_base = kSpan * floor(y / kSpan);
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(1.0);
        glBegin(GL_POINTS);
        for (int i=0; i<kNumNearPoints; i+=2) {
            for (int rx=-1; rx<2; ++rx)
            for (int ry=-1; ry<2; ++ry)
            glVertex2d(
                region_x_base + (double)rx * kSpan + near_points_[i],
                region_y_base + (double)ry * kSpan + near_points_[i+1]
            );
        }
        glEnd();

        // Far layer.
        region_x_base = kSpan * floor(kFarFactor * x / kSpan);
        region_y_base = kSpan * floor(kFarFactor * y / kSpan);
        glPushMatrix();
        glLoadIdentity();
        glRotated(angle, 0.0, 0.0, -1.0);
        glTranslated(-kFarFactor * x, -kFarFactor * y, 0.0);
        glColor3f(0.8, 0.8, 0.8);
        glPointSize(3.0);
        glBegin(GL_POINTS);
        for (int i=0; i<kNumFarPoints; i+=2) {
            for (int rx=-1; rx<2; ++rx)
            for (int ry=-1; ry<2; ++ry)
            glVertex2d(
                region_x_base + (double)rx * kSpan + near_points_[i],
                region_y_base + (double)ry * kSpan + near_points_[i+1]
            );
        }
        glEnd();
        glPopMatrix();
    }
};

#endif // BACKGROUND_H_
