#ifndef BIG_BANG_H_
#define BIG_BANG_H_

#include "ParticleManager.h"

class Star : public Particle {
public:
    Star(float L) : Particle(L) {}
    ~Star() {}
    void Update(double time_step) {
        Particle::Update(time_step);
        Fade();
        size += 0.2;
    }
};

class BigBang : public ParticleManager {
public:
    BigBang(int n, int m)
    : ParticleManager(n, m)
    {}
    ~BigBang() {}
    void AddParticle() {
        Star *p;
        p = new Star(5.0);
        double a = drand48() * 2.0 * M_PI;
        double s = drand48() * 10.0;
        p->vx = s * cos(a);
        p->vy = s * sin(a);
        particles.push_back(p);
    }
};

#endif // BIG_BANG_H_

