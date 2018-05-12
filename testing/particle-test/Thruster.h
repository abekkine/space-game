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
    Thruster(double period) : ParticleManager(period) {
        Position(10.0, 10.0);
        Angle(90.0);
        min_speed_ = 5.0;
        max_speed_ = 15.0;
        beam_angle_ = 45.0 * M_PI / 180.0;
    }
    ~Thruster() {}
    void SpeedRange(double min, double max) {
        min_speed_ = min;
        max_speed_ = max;
    }
    void BeamAngle(double alpha) {
        beam_angle_ = alpha * M_PI / 180.0;
    }
    void AddParticle() {
        Smoke *p;
        p = new Smoke(1.0);
        double a = (drand48() * beam_angle_) - 0.5 * beam_angle_;
        double s = drand48() * (max_speed_ - min_speed_) + min_speed_;
        p->vx = s * cos(a);
        p->vy = s * sin(a);
        p->color[0] = 1.0;
        p->color[1] = 0.5;
        p->color[2] = 0.0;

        particles.push_back(p);
    }
private:
    double min_speed_;
    double max_speed_;
    double beam_angle_;
};

#endif // THRUSTER_H_

