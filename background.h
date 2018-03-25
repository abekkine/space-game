#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "player.h"

// Render at x = 3995.42, y = -13099.8
// span = 100.0
// x' = 3900.0, y' = -13000.0
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
    , kFarFactor(0.6)
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
    void Render(const GameData::Player & p) {

        if (near_points_ == 0 || far_points_ == 0) return;

        // Near layer.
        double region_x_base = kSpan * floor(p.x / kSpan);
        double region_y_base = kSpan * floor(p.y / kSpan);
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
        region_x_base = kSpan * floor(kFarFactor * p.x / kSpan);
        region_y_base = kSpan * floor(kFarFactor * p.y / kSpan);
        glPushMatrix();
        glLoadIdentity();
        glRotated(p.angle, 0.0, 0.0, -1.0);
        glTranslated(-kFarFactor*p.x, -kFarFactor*p.y, 0.0);
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
