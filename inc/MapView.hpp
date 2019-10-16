#ifndef MAP_VIEW_HPP_
#define MAP_VIEW_HPP_

#include "UniverseInterface.h"
#include "Display.h"

#include <memory>

class MapView {
public:
    MapView() {
        size_ = 10.0;
        center_ = 0;
    }
    ~MapView() {}
    void Init(std::shared_ptr<UniverseInterface> universe) {
        universe_ = universe;
    }
    void Update(const double & center_x, const double & center_y) {
        if (universe_ == 0) {
            // TODO : properly handle exception.
            throw;
        }
        x_ = center_x;
        y_ = center_y;
        universe_->GetStars(x_, y_, size_, stars_);
        CalculateCenter();
    }
    void Render() {
        DISPLAY.UiMode();
        glPushMatrix();
        glLoadIdentity();
        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(50, 50);
        glVertex2i(50 + 200, 50);
        glVertex2i(50 + 200, 50 + 200);
        glVertex2i(50, 50 + 200);
        glEnd();
        if (universe_ != 0) {
            double sx, sy, sr;
            float * sColor;
            for (auto s : stars_) {
                s->GetPosition(sx, sy);
                sColor = s->GetColor();
                sr = s->GetRadius();
                glColor3fv(sColor);
                glPointSize(sr/500.0);
                glBegin(GL_POINTS);
                glVertex2i(50 + 20.0 * (5 + sx - x_), 50 + 20.0 * (5 + sy - y_));
                glEnd();
            }
        }
        glPopMatrix();
    }
    StarInterface * GetCenterStar() {
        return center_;
    }
    void CalculateCenter() {
        // Compute center star.
        if (stars_.empty()) {
            center_ = 0;
        } else {
            double minDistance = 100.0;
            double x, y, dx, dy, distance;
            for (auto s : stars_) {
                s->GetPosition(x, y);
                dx = x - x_; dy = y - y_;
                distance = dx*dx + dy*dy;
                if (distance < minDistance) {
                    minDistance = distance;
                    center_ = s;
                }
            }
        }
    }

private:
    double x_, y_;
    double size_;
    StarCollectionType stars_;
    std::shared_ptr<UniverseInterface> universe_;
    StarInterface * center_;
};

#endif // MAP_VIEW_HPP_
