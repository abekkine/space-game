#ifndef PLANET_STATION_H_
#define PLANET_STATION_H_

#include "station_interface.h"

class PlanetStation : public StationInterface {
public:
    PlanetStation()
    : vertices_{
        {-1.3, 0.5},
        {-1.7, 0.3},
        {-1.7, -1.2},
        {1.7, -1.2},
        {1.7, 0.3},
        {1.3, 0.5}}
    {}
    ~PlanetStation() {}
    void Init(b2World * world) {
        world_ = world;
        x_ = 0.0;
        y_ = 100.0;
        angle_ = 0.0;
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(x_, y_);
        body_ = world_->CreateBody(&bd);
        body_->SetUserData(this);
        
        b2PolygonShape shape;
        shape.Set(vertices_, 6);
        body_->CreateFixture(&shape, 1.0f);
    }
    void Attach(b2Body * body) {
        b2WeldJointDef wjd;
        wjd.bodyA = body;
        wjd.bodyB = body_;
        
        b2Vec2 worldAnchor = body_->GetWorldCenter();
        wjd.Initialize(body, body_, worldAnchor);
        wjd.localAnchorA = body->GetLocalPoint(worldAnchor);
        wjd.localAnchorB = body_->GetLocalPoint(worldAnchor);
        wjd.referenceAngle = 0.0;
        wjd.collideConnected = false;

        joint_ = (b2WeldJoint *) world_->CreateJoint(&wjd);
    }
    void Update(double time_step) {
        (void)time_step;
        b2Vec2 pos = body_->GetPosition();
        x_ = pos.x;
        y_ = pos.y;
        angle_ = body_->GetAngle() * 180.0 / M_PI;
    }
    void Render() {
        glPushMatrix();
        glTranslated(x_, y_, 0.0);
        glRotated(angle_, 0.0, 0.0, 1.0);
        glColor3f(0.8, 0.8, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(0.0, 0.0);
        for (int i=0; i<6; ++i) {
            glVertex2d(vertices_[i].x, vertices_[i].y);
        }
        glVertex2d(vertices_[0].x, vertices_[0].y);
        glEnd();
        glPopMatrix();
        Update(0.02);
    }
    void ProcessImpulse(float impulse) {
        (void)impulse;
    }
    void BeginContact(ContactInterface* object) {
        (void)object;
    }
    void EndContact(ContactInterface* object) {
        (void)object;
    }

private:
    b2Vec2 vertices_[6];
};

#endif // PLANET_STATION_H_

