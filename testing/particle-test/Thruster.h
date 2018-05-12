#ifndef THRUSTER_H_
#define THRUSTER_H_

#include "ParticleManager.h"

#include <time.h>

class Smoke : public Particle {
public:
    Smoke(float life) : Particle(life) {}
    ~Smoke() {}
    void Update(double time_step) {
        Particle::Update(time_step);
        Fade();
        size += 0.2;
    }
};

class Thruster : public ParticleManager {
public:
    Thruster(int n, int m) : ParticleManager(n, m) {
        x_ = 10.0;
        y_ = 10.0;
        angle_ = 90.0;
    }
    ~Thruster() {}
    void AddParticle() {
        Smoke *p;
        p = new Smoke(1.0);
        double a = (drand48() * M_PI / 6.0) - (M_PI / 12.0);
        double s = drand48() * 10.0 + 5.0;
        p->vx = s * cos(a);
        p->vy = s * sin(a);
        p->color[0] = 1.0;
        p->color[1] = 0.5;
        p->color[2] = 0.0;

        particles.push_back(p);
    }
};

#endif // THRUSTER_H_

