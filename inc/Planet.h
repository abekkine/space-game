#ifndef PLANET_H_
#define PLANET_H_

#include <GLFW/glfw3.h>
#include <Box2D.h>

#include <stdint.h>

class StationInterface;

class Planet {
public:
    Planet();
    ~Planet();
    void SetSeed(long int seed);
    double Mass();
    void SetPosition(double x, double y);
    void SetAngle(double value);
    void SetAngularVelocity(double value);
    void SetRadius(double value);
    void SetCoreRadius(double value);
    void SetColor(float r, float g, float b);
    void SetStation();
    b2Vec2 GetGravityAcceleration(b2Vec2 pos);
    b2Vec2 GetDistance(double x, double y);
    double GetSpanAngle(double x, double y);
    void Init(b2World * world);
    void Update();
    void Render();

private:
    long int seed_;
    const float kDefaultSurfaceFriction;
    double x_;
    double y_;
    double angle_;
    double angular_velocity_;
    double density_;
    double radius_;
    double core_radius_;
    float color_[3];
    StationInterface* station_;
    GLuint DL_surface_index_;
    GLuint DL_core_index_;

    b2World * world_;
    b2Body * physics_body_;

private:
    static uint32_t counter_;
    uint32_t identifier_;
};

#endif // PLANET_H_
