#ifndef PARTICLE_MANAGER_H_
#define PARTICLE_MANAGER_H_

#include "Particle.h"

#include <time.h>

#include <vector>

class ParticleManager {
public:
    ParticleManager(int numParticles, int numBurst)
    : kMaxParticles(numParticles)
    , kBurstSize(numBurst)
    {}
    virtual ~ParticleManager() {}
    virtual void Init() {
        srand48(clock());
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
    void AddParticles() {
        for (int i=0; i<kBurstSize; ++i) {
            AddParticle();
        }
    }
    virtual void Update(double time_step) {

        if (particles.size() < (kMaxParticles-kBurstSize)) {
            AddParticles();
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
    const int kBurstSize;
    double x_;
    double y_;
    double angle_;
    std::vector<Particle *> particles;
};

#endif // PARTICLE_MANAGER_H_

