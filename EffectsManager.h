#ifndef EFFECTS_MANAGER_H_
#define EFFECTS_MANAGER_H_

#include "StorableObject.h"
#include "RandomGenerator.hpp"

#include <Box2D/Box2D.h>

// TODO : Rename to .hpp ??
class EffectsManager : public StorableObject {
public:
    EffectsManager();
    ~EffectsManager();
    void Init();
    void Update(double time_step);
    void Render();
    // TODO : (#144) EffectManager & Thrust Particles will be refactored properly.
    struct ThrustParticle {
        ThrustParticle()
        : x(0.0), y(0.0), vx(0.0), vy(0.0), life(0.0) {}
        ~ThrustParticle() {
            R_.SetRange(-M_PI / 12.0, M_PI / 12.0);
        }
        void SetPosition(b2Vec2 position) {
            x = position.x;
            y = position.y;
            life = 0.25;
        }
        void SetThrust(b2Vec2 thrust, b2Vec2 velocity) {
            double ta = atan2(thrust.y, thrust.x) - M_PI;
            ta = fmod(ta, 2.0 * M_PI);
            //double td = (drand48() * M_PI / 6.0) - M_PI / 12.0;
            double td = R_.GetValue();
            vx = velocity.x + 1.00 * thrust.Length() * cos(ta + td);
            vy = velocity.y + 1.00 * thrust.Length() * sin(ta + td);
        }
        void Update(double time_step) {
            if (life > 0.0) {
                life -= time_step;
                x += time_step * vx;
                y += time_step * vy;
            }
        }
        double x, y;
        double vx, vy;
        double life;
        RandomGenerator R_;
    };

    void MainThruster(b2Vec2 thrust, b2Vec2 position, b2Vec2 velocity);
    void BowThruster(double moment, double angle, b2Vec2 position, b2Vec2 velocity);
    void UpdateThrust(double time_step);
    void RenderThrust();

private:
    int bow_index;
    ThrustParticle thrust_particles_[100];
    int tp_index_ = 0;
};

#endif // EFFECTS_MANAGER_H_
