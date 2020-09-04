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
        size_ = kMapRange * 1.5;
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
        map_stars_.clear();
        x_ = center_x;
        y_ = center_y;
        universe_->GetStars(x_, y_, size_, stars_);
        CalculateCenter();
        CalculateMapStars();
    }
    void RenderMapFrame() {
        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(kMapX, kMapY);
        glVertex2i(kMapX + kMapSize, kMapY);
        glVertex2i(kMapX + kMapSize, kMapY + kMapSize);
        glVertex2i(kMapX, kMapY + kMapSize);
        glEnd();
    }
    void RenderMapBackground() {
        glColor4f(0.0, 0.0, 0.2, 0.8);
        glBegin(GL_QUADS);
        glVertex2i(kMapX, kMapY);
        glVertex2i(kMapX + kMapSize, kMapY);
        glVertex2i(kMapX + kMapSize, kMapY + kMapSize);
        glVertex2i(kMapX, kMapY + kMapSize);
        glEnd();
    }
    void GetMapXY(StarInterface * s, int & x, int & y) {
        double sx, sy;
        s->GetPosition(sx, sy);
        x = starXToMapX(sx - x_);
        y = starYToMapY(sy - y_);
    }
    void RenderMapCenter() {
        int centerX, centerY;
        GetMapXY(center_, centerX, centerY);
        glColor4f(1.0, 1.0, 1.0, 0.3);
        glLineWidth(0.5);
        glBegin(GL_LINES);
        glVertex2i(kMapX, centerY);
        glVertex2i(kMapX + kMapSize, centerY);
        glVertex2i(centerX, kMapY);
        glVertex2i(centerX, kMapY + kMapSize);
        glEnd();
    }
    void RenderStars() {
        if (universe_ != 0) {
            double sx, sy, sr;
            float * sColor;
            int sMapX, sMapY;
            for (auto s : map_stars_) {
                GetMapXY(s, sMapX, sMapY);
                sColor = s->GetColor();
                glColor3fv(sColor);
                glPointSize(4.0);
                glBegin(GL_POINTS);
                glVertex2i(sMapX, sMapY);
                glEnd();
            }
        }
    }
    void RenderSelection() {
        // DEBUG selected
        if (isStarInMap(dbg_dst_x_ - x_, dbg_dst_y_ - y_) == false) {
            return;
        }
        glColor4f(1.0, 0.0, 0.0, 0.5);
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glVertex2i(
            starXToMapX(dbg_dst_x_ - x_),
            starYToMapY(dbg_dst_y_ - y_)
        );
        glEnd();
    }
    void Render() {
        DISPLAY.UiMode();
        glPushMatrix();
        glLoadIdentity();
        RenderMapBackground();
        RenderMapCenter();
        RenderStars();
        RenderSelection();
        RenderMapFrame();
        glPopMatrix();
    }

    void CalculateMapLimits() {
        const double mapLimitRatio = 0.98;
        star_max_x_ = mapLimitRatio * mapXToStarX(kMapX);
        star_min_x_ = mapLimitRatio * mapXToStarX(kMapX + kMapSize);
        star_min_y_ = mapLimitRatio * mapYToStarY(kMapY);
        star_max_y_ = mapLimitRatio * mapYToStarY(kMapY + kMapSize);
    }
    bool isStarInMap(const double & x, const double & y) {
        if (x > star_min_x_ && x < star_max_x_ &&
            y > star_min_y_ && y < star_max_y_) {
            
            return true;
        } else {
            return false;
        }
    }
    void CalculateMapStars() {
        CalculateMapLimits();

        double sx, sy, sxRel, syRel;
        for (auto s : stars_) {
            s->GetPosition(sx, sy);
            sxRel = sx - x_;
            syRel = sy - y_;
            if (isStarInMap(sxRel, syRel)) {
                map_stars_.push_back(s);
            }
        }
    }
    int starXToMapX(const double & x) {
        int mapX = kMapX + 0.5 * kMapSize - kMapScale * x;
        return mapX;
    }
    int starYToMapY(const double & y) {
        int mapY = kMapY + 0.5 * kMapSize + kMapScale * y;
        return mapY;
    }
    double mapXToStarX(const int x) {
        double starX = (kMapX + 0.5 * kMapSize - x) / kMapScale;
        return starX;
    }
    double mapYToStarY(const int y) {
        double starY = (y - kMapY - 0.5 * kMapSize) / kMapScale;
        return starY;
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
    StarCollectionType map_stars_;
    std::shared_ptr<UniverseInterface> universe_;
    StarInterface * center_;
    double star_max_x_, star_max_y_;
    double star_min_x_, star_min_y_;
};

#endif // MAP_VIEW_HPP_
