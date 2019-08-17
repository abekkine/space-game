#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "UniverseInterface.h"
#include "UniverseParameters.hpp"
#include "StarCategory.h"
#include "StarInterface.h"
#include "StarInfo.h"
#include "StarCollection.hpp"

#include "PerlinNoise.hpp"

class Universe : public UniverseInterface {
public:
    std::shared_ptr<StarCollection> GetStars();
    void UpdateStars(const double & centerX, const double & centerY, const double & distance);
    void Save();
    void Load();
    void * GetParameters() {
        return (void *)&m_params;
    }

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

private:
    void UpdateCategoryIndex();
    int GetCategoryIndex(double value);
    bool GenerateStarAt(const double & x, const double & y, StarInfo * p);

private:
    siv::PerlinNoise * m_noise;
    // Input parameters
    UniverseParameters m_params;
    // Force star generation after parameter update
    std::shared_ptr<StarCollection> m_stars;
};

#endif // UNIVERSE_H
