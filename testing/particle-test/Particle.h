#ifndef PARTICLE_H_
#define PARTICLE_H_

class Particle {
public:
    Particle(float lifetime)
    : totalLife(lifetime)
    , life(lifetime)
    , size(5.0)
    , color{1.0, 1.0, 1.0, 1.0}
    , x(0.0), y(0.0)
    , vx(0.0), vy(0.0)
    , ax(0.0), ay(0.0)
    {}
    virtual ~Particle() {}
    void Render() {
        if (life > 0.0) {
            glPointSize(size);
            glColor4fv(color);
            glBegin(GL_POINTS);
            glVertex2f(x, y);
            glEnd();
        }
    }
    virtual void Update(double time_step) {
        x += vx * time_step;
        y += vy * time_step;
        vx += ax * time_step;
        vy += ay * time_step;
        life -= time_step;
    }
    void Fade() {
        color[3] = life / totalLife;
    }

    float totalLife;
    float life;
    float size;
    float color[4];

    float x, y;
    float vx, vy;
    float ax, ay;
};

#endif // PARTICLE_H_

