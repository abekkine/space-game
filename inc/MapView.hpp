#ifndef MAP_VIEW_HPP_
#define MAP_VIEW_HPP_

#include "UniverseInterface.h"
#include "Display.h"

#include <memory>

// DEBUG
extern double dbg_dst_x_, dbg_dst_y_;

class MapView {
public:
    MapView()
    : kMapSize(400)
    , kMapX(50), kMapY(50)
    , kMapRange(10.0)
    , kMapScale(kMapSize / kMapRange)
    {
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
        glVertex2i(kMapX, kMapY);
        glVertex2i(kMapX + kMapSize, kMapY);
        glVertex2i(kMapX + kMapSize, kMapY + kMapSize);
        glVertex2i(kMapX, kMapY + kMapSize);
        glEnd();
        if (universe_ != 0) {
            double sx, sy, sr;
            float * sColor;
            for (auto s : stars_) {
                s->GetPosition(sx, sy);
                sColor = s->GetColor();
                glColor3fv(sColor);
                glPointSize(4.0);
                glBegin(GL_POINTS);
                glVertex2i(kMapX + kMapSize - kMapScale * (0.5 * kMapRange + sx - x_), kMapY + kMapScale * (0.5 * kMapRange + sy - y_));
                glEnd();
            }
        }
        // DEBUG selected
        glColor4f(1.0, 0.0, 0.0, 0.5);
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glVertex2i(kMapX + kMapSize - kMapScale * (0.5 * kMapRange + dbg_dst_x_ - x_), kMapY + kMapScale * (0.5 * kMapRange + dbg_dst_y_ - y_));
        glEnd();
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
    const int kMapSize;
    const int kMapX, kMapY;
    const double kMapRange;
    const double kMapScale;
    double x_, y_;
    double size_;
    StarCollectionType stars_;
    std::shared_ptr<UniverseInterface> universe_;
    StarInterface * center_;
};

#endif // MAP_VIEW_HPP_
