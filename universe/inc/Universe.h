#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "UniverseInterface.h"
#include "UniverseParameters.hpp"
#include "StarCategory.h"
#include "StarInterface.h"
#include "StarInfo.h"

#include <vector>
#include <noise.h>

class Universe : public UniverseInterface {
public:
    void GetStars(const double & centerX, const double & centerY, const double & distance, StarCollectionType & stars);

private:
    enum ExtentIndexEnum {
        eiSize = 0,
        eiBaseX = 1,
        eiBaseY = 2,
        eiMAX = 3,
    };
    int32_t extent_indexes_[2 * eiMAX];

public: // setters
    void setOctaveCount(const double & value);
    double getOctaveCount();
    void setFrequency(const double & value);
    double getFrequency();
    void setStepSize(const double & value);
    double getStepSize();
    void setXPosition(const double & value);
    double getXPosition();
    void setYPosition(const double & value);
    double getYPosition();
    void setMinValue(const double & value);
    double getMinValue();
    void setZIndex(const double & value);
    double getZIndex();

public:
    Universe();
    ~Universe();
    void GetParameters(UniverseParameters & params) {
        params = m_params;
    }
    void SetOctaveCount();

    UniverseParameters * getUniverseParams() {
        return &m_params;
    }

private:
    void UpdateCategoryIndex();
    int GetCategoryIndex(double value);
    bool GenerateStarAt(const double & x, const double & y, StarInfo * p);

private:
    noise::module::Perlin m_noise;
    // noise::module::Billow m_noise;
    // noise::module::RidgedMulti m_noise;
    StarCollectionType m_stars;
    // Input parameters
    UniverseParameters m_params;
    // Force star generation after parameter update
    bool force_update_;
};

#endif // UNIVERSE_H
