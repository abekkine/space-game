#include "Universe.h"

#include <math.h>

#include <iostream>
#include <random>

extern StarCategory star_categories_[e_NUM_CATEGORIES];

Universe::Universe() {

    m_noise = new siv::PerlinNoise(123U);

    // DEBUG
    // for (int i=0; i<eiMAX; ++i) {
    //     extent_indexes_[(i << 1) + 0] = 0;
    //     extent_indexes_[(i << 1) + 1] = 0;
    // }
    UpdateCategoryIndex();

    Load();
}

Universe::~Universe() {}

void Universe::UpdateCategoryIndex() {
#ifndef USE_REAL_ABUNDANCES
    const double base = 1.2;
    const double scaleFactor = (base - 1.0) / (pow(base, (double)(e_NUM_CATEGORIES)) - 1.0);
    double power = 0.0;
    double sum = 0.0;
    for (int i=e_NUM_CATEGORIES-1; i>0; --i) {
        sum += pow(base, (double)i);
        star_categories_[e_NUM_CATEGORIES - i - 1].abundance = sum * scaleFactor;
    }
    star_categories_[e_NUM_CATEGORIES - 1].abundance = 1.0;
#endif // USE_REAL_ABUNDANCES
}

bool Universe::GenerateStarAt(const double & x, const double & y, StarInfo * p) {
    std::mt19937 gen(1000000.0 * p->seed);
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    int category_index;

    p->x = x + m_params.stepSize.value * dis(gen);
    p->y = y + m_params.stepSize.value * dis(gen);
    category_index = GetCategoryIndex(p->seed);
    double rMin = star_categories_[category_index].minRadius;
    double rMax = star_categories_[category_index].maxRadius;
    double mMin = star_categories_[category_index].minMass;
    double mMax = star_categories_[category_index].maxMass;
    p->radius = 50.0 * (rMax + rMin + dis(gen) * (rMax - rMin));
    p->mass = 0.5 * (mMax + mMin + dis(gen) * (mMax - mMin));
    p->cat_name = star_categories_[category_index].name;
    p->cat_type = star_categories_[category_index].type;
    p->color_ptr = star_categories_[category_index].baseColor;
}

void Universe::GetStars(
    const double & centerX,
    const double & centerY,
    const double & distance,
    StarCollectionType & stars) {

    const double ds = m_params.stepSize.value;
    // Limit query distance.
    double effective_distance;
    if (distance > 20.0 * ds) {
        effective_distance = 20.0 * ds;
    } else {
        effective_distance = distance;
    }

    // BUG : Star generation needs a fix for panning.
    const double frame_size = ds * floor(0.5 * effective_distance / ds);
    const double base_x = ds * floor(centerX / ds);
    const double base_y = ds * floor(centerY / ds);

    m_stars.clear();
    double f = frame_size / m_params.frequency.value;
    for (double dx=-frame_size; dx<frame_size; dx+=ds) {
        for (double dy=-frame_size; dy<frame_size; dy+=ds) {
            double x = base_x + dx;
            double y = base_y + dy;

            double value = m_noise->octaveNoise0_1(x, y, m_params.zIndex.value, m_params.octaveCount.value);

            if (value > m_params.minValue.value) {
                StarInfo * p = new StarInfo();
                p->seed = value / (1.0 - m_params.minValue.value);
                GenerateStarAt(x, y, p);
                m_stars.push_back(p);
            }
        }
    }

    stars = m_stars;
}

int Universe::GetCategoryIndex(double value) {
    int ix = e_NUM_CATEGORIES-1;
    for (int i=0; i<e_NUM_CATEGORIES-1; ++i) {
        if (value < star_categories_[i].abundance) {
            ix = i;
            break;
        }
    }
    return ix;
}

void Universe::Save() {
    m_params.Save();
}

void Universe::Load() {
    m_params.Load();
    m_params.Update();
}
