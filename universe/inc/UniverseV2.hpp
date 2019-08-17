#ifndef UNIVERSE_V2_HPP_
#define UNIVERSE_V2_HPP_

#include "UniverseInterface.h"
#include "UniverseParameters.hpp"
#include "StarInfo.h"
#include "PerlinNoise.hpp"

#include <memory>

class UniverseV2 : public UniverseInterface {
public:
    UniverseV2() {
        Defaults();
    }
    ~UniverseV2() {}
    std::shared_ptr<StarCollection> GetStars() {
        return stars_;
    }
    void UpdateStars(
        const double & centerX, const double & centerY,
        const double & distance
    ) {
        const double dS = params_.stepSize.value;
        int32_t eSize = floor(0.5 * distance / dS);
        int eBaseX = floor(centerX / dS);
        int eBaseY = floor(centerY / dS);
        double frame_size = dS * eSize;
        double base_x = dS * eBaseX;
        double base_y = dS * eBaseY;
        double f = frame_size / params_.frequency.value;
        stars_->Reset();
        for (double dx=-frame_size; dx<=frame_size; dx+=dS) {
            for (double dy=-frame_size; dy<=frame_size; dy+=dS) {
                double x = base_x + dx;
                double y = base_y + dy;
                const double f = params_.frequency.value;
                double value = noise_->octaveNoise0_1(x/f, y/f, params_.zIndex.value, params_.octaveCount.value);
                stars_->AddStar(x, y, value);
            }
        }
    }
    void * GetParameters() {
        return (void *)&params_;
    }
    void Save() {
        params_.Save();
    }
    void Load() {
        params_.Load();
    }

private:
    void Defaults() {
        noise_ = new siv::PerlinNoise(414212U);
        stars_.reset(new StarCollection());
    }

private:
    UniverseParameters params_;
    siv::PerlinNoise * noise_;
    std::shared_ptr<StarCollection> stars_;
};

#endif // UNIVERSE_V2_HPP_
