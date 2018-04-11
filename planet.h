#ifndef PLANET_H_
#define PLANET_H_

#include <GLFW/glfw3.h>
#include <Box2D.h>

#include "game_definitions.h"

class Planet {
public:
    Planet()
    : x_(0.0)
    , y_(0.0)
    , angle_(0.0)
    , mass_(1.0)
    , density_(1.0)
    , radius_(50.0)
    , color_{1.0, 0.0, 0.0}
    {
        identifier_ = counter_++;
    }
    ~Planet() {}
    double Mass() {
        return M_PI * radius_ * radius_ * density_;
    }
    void SetPosition(double x, double y) {
        x_ = x;
        y_ = y;
    }
    void SetAngle(double value) {
        angle_ = value;
    }
    void SetRadius(double value) {
        radius_ = value;
    }
    b2Vec2 GetGravityFactor(double x, double y) {
        double dx = x_ - x;
        double dy = y_ - y;
        double r2 = dx*dx + dy*dy;
        double r = sqrt(r2);

        double gmr2 = GameDefinitions::kGravityConstant * Mass() / (r2 * r);
        double fx = gmr2 * dx;
        double fy = gmr2 * dy;

        return b2Vec2(fx, fy);
    }
    b2Vec2 GetDistance(double x, double y) {
        return b2Vec2(x_ - x, y_ - y);
    }
    b2Vec2 GetHorizonVector(double dir, double x, double y) {
        b2Vec2 d(x_ - x, y_ - y);
        double dlen = d.Length();
        d = (1.0 / dlen) * d;
        double sint = radius_ / dlen;
        double cost = sqrt(1.0 - sint*sint);
        return b2Vec2(
            d.x * cost + d.y * sint * dir,
            -d.x * sint * dir + d.y * cost
        );
    }
    void Init(b2World * world) {
        b2BodyDef def;
        def.type = b2_kinematicBody;
        def.position.Set(x_, y_);
        physics_body_ = world->CreateBody(&def);

        b2CircleShape shape;
        // [TODO] : why (0.0, 0.0)
        shape.m_p.Set(0.0, 0.0);
        shape.m_radius = radius_;

        b2FixtureDef fixture;
        fixture.density = density_;
        fixture.friction = 0.7;

        fixture.shape = &shape;
        physics_body_->CreateFixture(&fixture);
    }
    void Update() {
        physics_body_->SetAngularVelocity(0.2 * M_PI / 180.0);
        b2Vec2 pos = physics_body_->GetPosition();
        x_ = pos.x;
        y_ = pos.y;
        angle_ = physics_body_->GetAngle() * 180.0 / M_PI;
    }
    void Render() {
        glTranslated(x_, y_, 0.0);
        glRotated(angle_, 0.0, 0.0, 1.0);
        glColor3fv(color_);

        const double R = radius_;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(0.0, 0.0);
        for (double a=0.0; a < 2.0 * M_PI; a+=0.05) {
            glVertex2d(R * cos(a), R * sin(a));
        }
        glVertex2d(R, 0.0);
        glEnd();

        glPointSize(3.0);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2d(0.0, R*0.99);
        glEnd();
    }
private:
    double x_;
    double y_;
    double angle_;
    double mass_;
    double density_;
    double radius_;
    float color_[3];

    b2Body * physics_body_;
private:
    static uint32_t counter_;
    uint32_t identifier_;
};

#endif // PLANET_H_
