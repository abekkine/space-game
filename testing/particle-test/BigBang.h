#ifndef BIG_BANG_H_
#define BIG_BANG_H_

#include "ParticleManager.h"

#include <time.h>

class Star : public Particle {
public:
    Star(float L) : Particle(L) {}
    ~Star() {}
    void Update(double time_step) {
        Particle::Update(time_step);
        for (int i=0; i<3; ++i) {
            color[i] = life / totalLife;
        }
        size += 0.1;
    }
};

class BigBang : public ParticleManager {
public:
    BigBang(int n, int m)
    : ParticleManager(n, m)
    {}
    ~BigBang() {}
    void Init() {
        srand48(clock());
    }
    void AddParticle() {
        Star *p;
        for (int i=0; i<kBurstSize; ++i) {
            p = new Star(5.0);
            double a = drand48() * 2.0 * M_PI;
            double s = drand48() * 10.0;
            p->vx = s * cos(a);
            p->vy = s * sin(a);
            particles.push_back(p);
        }
    }
    void Update(double time_step) {
        ParticleManager::Update(time_step);
    }
};

#endif // BIG_BANG_H_

