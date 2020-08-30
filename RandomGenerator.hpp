#ifndef RANDOM_GENERATOR_HPP
#define RANDOM_GENERATOR_HPP

#include <random>

class RandomGenerator {
public:
    RandomGenerator() {
        std::random_device rd;
        generator_ = std::mt19937(rd());
        min_ = 0.0;
        max_ = 1.0;
    }
    RandomGenerator(uint32_t seed) {
        generator_ = std::mt19937(seed);
        min_ = 0.0;
        max_ = 1.0;
    }
    ~RandomGenerator() {}
    void SetRange(const double & min, const double & max) {
        min_ = min;
        max_ = max;
    }
    double GetValue() {
        std::uniform_real_distribution<double> d(min_, max_);
        return d(generator_);
    }

private:
    double min_, max_;
    std::mt19937 generator_;
};

#endif // RANDOM_GENERATOR_HPP
