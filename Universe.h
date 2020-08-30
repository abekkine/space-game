#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "UniverseInterface.h"
#include "UniverseParameters.hpp"
#include "StarCategory.h"
#include "StarInterface.h"
#include "StarInfo.h"

#include "PerlinNoise.hpp"

#include <vector>

class Universe : public UniverseInterface {
public:
    void GetStars(const double & centerX, const double & centerY, const double & distance, StarCollectionType & stars);
    void Save();
    void Load();

private:
    enum ExtentIndexEnum {
        eiSize = 0,
        eiBaseX = 1,
        eiBaseY = 2,
        eiMAX = 3,
    };
    int32_t extent_indexes_[2 * eiMAX];

public:
    Universe();
    ~Universe();
    UniverseParameters * GetParameters() {
        return &m_params;
    }

private:
    void UpdateCategoryIndex();
    int GetCategoryIndex(double value);
    bool GenerateStarAt(const double & x, const double & y, StarInfo * p);

private:
    siv::PerlinNoise * m_noise;
    StarCollectionType m_stars;
    // Input parameters
    UniverseParameters m_params;
    // Force star generation after parameter update
};

#endif // UNIVERSE_H
