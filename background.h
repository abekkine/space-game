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
public:
    Background()
    : kSpan(100.0)
    , kStep(10.0)
    , kFarFactor(0.6)
    {}
    ~Background() {}
    void Init() {

    }
    void Render(const GameData::Player & p) {

        // Near layer.
        double region_x_base = kSpan * floor(p.x / kSpan);
        double region_y_base = kSpan * floor(p.y / kSpan);
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(1.0);
        glBegin(GL_POINTS);
        for(double x = 0.0;  x < kSpan; x += kStep) {
            for(double y = 0.0; y < kSpan; y += kStep) {
                for (int rx=-1; rx<2; ++rx)
                for (int ry=-1; ry<2; ++ry)
                glVertex2d(
                    region_x_base + (double)rx * kSpan + x,
                    region_y_base + (double)ry * kSpan + y
                );
            }
        }
        glEnd();

        // Far layer.
        region_x_base = kSpan * floor(kFarFactor * p.x / kSpan);
        region_y_base = kSpan * floor(kFarFactor * p.y / kSpan);
        glPushMatrix();
        glLoadIdentity();
        glRotated(p.a, 0.0, 0.0, -1.0);
        glTranslated(-kFarFactor*p.x, -kFarFactor*p.y, 0.0);
        glColor3f(0.5, 0.5, 0.5);
        glPointSize(2.0);
        glBegin(GL_POINTS);
        for(double x = 0.0; x < kSpan; x += kFarFactor * kStep) {
            for(double y = 0.0; y < kSpan; y += kFarFactor * kStep) {
                for (int rx=-1; rx<2; ++rx)
                for (int ry=-1; ry<2; ++ry)
                glVertex2d(
                    region_x_base + (double)rx * kSpan + x,
                    region_y_base + (double)ry * kSpan + y
                );
            }
        }
        glEnd();
        glPopMatrix();
    }
};

#endif // BACKGROUND_H_
