#ifndef COLLISION_HANDLER_H_
#define COLLISION_HANDLER_H_

#include <Box2D.h>

class CollisionHandler : public b2ContactListener {
public:
    CollisionHandler();
    ~CollisionHandler();

    void PostSolve(b2Contact * contact, const b2ContactImpulse* impulse);
    void BeginContact(b2Contact * contact);
    void EndContact(b2Contact * contact);
};

#endif // COLLISION_HANDLER_H_

