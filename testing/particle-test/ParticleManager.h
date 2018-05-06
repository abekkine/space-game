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
        for (auto p : particles) {
            p->Render();
        }
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
    std::vector<Particle *> particles;
};

#endif // PARTICLE_MANAGER_H_

