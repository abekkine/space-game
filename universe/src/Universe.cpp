#include "Universe.h"

#include <math.h>

#include <iostream>
#include <random>

extern StarCategory star_categories_[e_NUM_CATEGORIES];

Universe::Universe() {

    m_noise = new siv::PerlinNoise(123U);

    for (int i=0; i<eiMAX; ++i) {
        extent_indexes_[(i << 1) + 0] = 0;
        extent_indexes_[(i << 1) + 1] = 0;
    }

    UpdateCategoryIndex();
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
    p->radius = 0.01 * 0.5 * (rMax + rMin + dis(gen) * (rMax - rMin));
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

    if (!m_params.CheckUpdate()) {
        extent_indexes_[eiSize] = static_cast<int32_t>(floor(0.5 * distance / ds));
        extent_indexes_[eiBaseX] = static_cast<int>(floor(centerX/ds));
        extent_indexes_[eiBaseY] = static_cast<int>(floor(centerY/ds));

        bool update_stars = false;
        for (int i=0; i<eiMAX; ++i) {
            if (extent_indexes_[i + 0] ^ extent_indexes_[i + eiMAX]) {
                update_stars = true;
                break;
            }
        }

        for (int i=0; i<eiMAX; ++i) {
            extent_indexes_[i + eiMAX] = extent_indexes_[i + 0];
        }

        if (update_stars == false) {
            return;
        }
    }

    double frame_size = ds * static_cast<double>(extent_indexes_[eiSize]);
    const double base_x = ds * static_cast<double>(extent_indexes_[eiBaseX]);
    const double base_y = ds * static_cast<double>(extent_indexes_[eiBaseY]);

    // Limit rendering work
    if ( 2.0 * frame_size / ds > 200.0 ) {
        frame_size = 100.0 * ds;
    }

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
}
