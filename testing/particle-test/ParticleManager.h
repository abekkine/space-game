#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Particle.h"

#include <assert.h>
#include <time.h>

#include <vector>

#define MAX_PARTICLES 1000

class ParticleManager {
public:
    ParticleManager(double period)
    : kMaxParticles(MAX_PARTICLES)
    , period_(period)
    , remainder_time_(0.0)
    {}
    virtual ~ParticleManager() {}
    virtual void Init() {
        srand48(clock());
    }
    void Period(double period) {
        period_ = period;
    }
    void Position(double x, double y) {
        x_ = x;
        y_ = y;
    }
    void Angle(double a) {
        angle_ = a;
    }
    virtual void AddParticle() = 0;
    void Render() {
        glPushMatrix();
        glLoadIdentity();
        glTranslated(x_, y_, 0.0);
        glRotated(angle_, 0.0, 0.0, 1.0);

        for (auto p : particles) {
            p->Render();
        }

        glPopMatrix();
    }
    virtual void Update(double time_step) {

        assert( period_ > 0.0 && "Period cannot be equal to or lesser than 0.0");

        if (period_ > 0.0) {

            int count = (time_step + remainder_time_) / period_;
            remainder_time_ = fmod(time_step + remainder_time_, period_);
            for (int i=0; i<count; ++i) {
                if (particles.size() < kMaxParticles) {
                    AddParticle();
                }
            }
        }

        for (auto iP = particles.begin(); iP != particles.end(); ++iP) {
            if ((*iP)->life < 0.0) {
                delete *iP;
                iP = particles.erase(iP);
            }
            else {
                (*iP)->Update(time_step);
            }
        }
    }

protected:
    const int kMaxParticles;
    double period_;
    double x_;
    double y_;
    double angle_;
    std::vector<Particle *> particles;
    double remainder_time_;
};

#endif // PARTICLE_MANAGER_H_

