#include "EffectsManager.h"
#include "Display.h"

#include <unistd.h>
#include <math.h>

EffectsManager::EffectsManager() {
    bow_index = 0;
}
EffectsManager::~EffectsManager() {}
void EffectsManager::Init() {}
void EffectsManager::Update(double time_step) {
    UpdateThrust(time_step);
}
void EffectsManager::Render() {
    RenderThrust();
}
void EffectsManager::MainThruster(b2Vec2 thrust, b2Vec2 position, b2Vec2 velocity) {
    if (thrust.Length() > 0.0) {
        thrust_particles_[tp_index_].SetPosition(position);
        thrust_particles_[tp_index_].SetThrust(thrust, velocity);
        tp_index_++;
        tp_index_ = tp_index_ % 100;
    }
}
void EffectsManager::BowThruster(double moment, double angle, b2Vec2 position, b2Vec2 velocity) {
    bow_index++;
    bow_index &= 0x3;
    if (bow_index != 0) return;

    double abs_moment = fabs(moment);
    if (abs_moment > 0.0) {
        b2Vec2 t;
        b2Vec2 p;
        double a = 0.0;
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
void EffectsManager::UpdateThrust(double time_step) {

    for (int i=0; i<100; ++i) {
        thrust_particles_[i].Update(time_step);
    }
}
void EffectsManager::RenderThrust() {

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
