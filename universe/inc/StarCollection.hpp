#ifndef STAR_COLLECTION_HPP_
#define STAR_COLLECTION_HPP_

#include "StarInterface.h"
#include "StarInfo.h"

#include <vector>
#include <functional>
#include <memory>
#include <iostream>

#include <math.h>

class StarCollection {
public:
    StarCollection() {}
    ~StarCollection() {}
    void Reset() {
        for (auto s : stars_) {
            s.reset();
        }
        stars_.clear();
    }
    void AddStar(const double & x, const double & y, const double & value) {
        StarInfo * s = new StarInfo();
        s->x = x;
        s->y = y;
        s->radius = value;
        for (int i=0; i<3; ++i) s->color_ptr[i] = value;
        std::shared_ptr<StarInterface> si(s);
        stars_.push_back(si);
    }
    void RunOperation(std::function<void(std::shared_ptr<StarInterface>)> op) {
        for (auto s : stars_) {
            op(s);
        }
    }
    size_t GetSize() {
        return stars_.size();
    }
    std::shared_ptr<StarInterface> GetSelection(const double & x, const double & y, const double & vicinity) {
        for (auto s : stars_) {
            double sx, sy;
            s->GetPosition(sx, sy);
            double dx = x - sx;
            double dy = y - sy;
            double distance = sqrt(dx*dx + dy*dy);
            if (distance < vicinity) {
                return s;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::shared_ptr<StarInterface>> stars_;
};

#endif // STAR_COLLECTION_HPP_
