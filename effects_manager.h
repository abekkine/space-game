#ifndef EFFECTS_MANAGER_H_
#define EFFECTS_MANAGER_H_

#include <unistd.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <Box2D.h>

class EffectsManager {
public:
    EffectsManager() {}
    ~EffectsManager() {}
    void Init() {}
    void Update(double time_step) {
        UpdateThrust(time_step);
    }
    void Render() {
        RenderThrust();
    }
    // BEGIN Thruster
    struct ThrustParticle {
        ThrustParticle()
        : x(0.0), y(0.0), vx(0.0), vy(0.0), life(0.0) {}
        ~ThrustParticle() {}
        void SetPosition(b2Vec2 position) {
            x = position.x;
            y = position.y;
            life = 0.25;
        }
        void SetThrust(b2Vec2 thrust, b2Vec2 velocity) {
            double ta = atan2(thrust.y, thrust.x) - M_PI;
            ta = fmod(ta, 2.0 * M_PI);
            double td = (drand48() * M_PI / 6.0) - M_PI / 12.0;
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
    };
    ThrustParticle thrust_particles_[100];
    int tp_index_ = 0;
    void MainThruster(b2Vec2 thrust, b2Vec2 position, b2Vec2 velocity) {
        if (thrust.Length() > 0.0) {
            thrust_particles_[tp_index_].SetPosition(position);
            thrust_particles_[tp_index_].SetThrust(thrust, velocity);
            tp_index_++;
            tp_index_ = tp_index_ % 100;
        }
    }
    int bow_index;
    void BowThruster(double moment, double angle, b2Vec2 position, b2Vec2 velocity) {
        bow_index++;
        bow_index &= 0x3;
        if (bow_index != 0) return;

        double abs_moment = fabs(moment);
        if (abs_moment > 0.0) {
            b2Vec2 t;
            b2Vec2 p;
            double a;
            if (moment > 0.0) {
                a = (angle - 180.0) * M_PI / 180.0;
            } else if (moment < 0.0) {
                a = (angle) * M_PI / 180.0;
            }
            p.x = position.x + 1.0 * cos((angle + 90.0) * M_PI / 180.0);
            p.y = position.y + 1.0 * sin((angle + 90.0) * M_PI / 180.0);
            t.x = 20.0 * abs_moment * cos(a);
            t.y = 20.0 * abs_moment * sin(a);
            thrust_particles_[tp_index_].SetPosition(p);
            thrust_particles_[tp_index_].SetThrust(t, velocity);
            tp_index_++;
            tp_index_ = tp_index_ % 100;
        }
    }
    void UpdateThrust(double time_step) {

        for (int i=0; i<100; ++i) {
            thrust_particles_[i].Update(time_step);
        }
    }
    void RenderThrust() {

        glColor3f(1.0, 1.0, 1.0);
        glPointSize(3.0);
        glBegin(GL_POINTS);
        for (int i=0; i<100; ++i) {
            if (thrust_particles_[i].life > 0.0) {
                glColor4f(1.0, 1.0, 1.0, 4.0 * thrust_particles_[i].life);
                glVertex2d(thrust_particles_[i].x, thrust_particles_[i].y);
            }
        }
        glEnd();
    }
    // END Thruster
};

#endif // EFFECTS_MANAGER_H_

