#ifndef COLLISION_HANDLER_H_
#define COLLISION_HANDLER_H_

#include <Box2D.h>

#include "SpaceShip.h"

class CollisionHandler : public b2ContactListener {
public:
    CollisionHandler() {}
    ~CollisionHandler() {
        puts("~CollisionHandler()");
    }

    void PostSolve(b2Contact * contact, const b2ContactImpulse* impulse) {

        int count = contact->GetManifold()->pointCount;
        float maxImpulse = 0.0;
        for (int i=0; i<count; ++i) {
            maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
        }

        ContactInterface* objA = static_cast<ContactInterface *>(contact->GetFixtureA()->GetBody()->GetUserData());
        ContactInterface* objB = static_cast<ContactInterface *>(contact->GetFixtureB()->GetBody()->GetUserData());

        if (objA != 0) {
            objA->ProcessImpulse(maxImpulse);
        }

        if (objB != 0) {
            objB->ProcessImpulse(maxImpulse);
        }
    }

    void BeginContact(b2Contact * contact) {

        ContactInterface* objA = static_cast<ContactInterface *>(contact->GetFixtureA()->GetBody()->GetUserData());
        ContactInterface* objB = static_cast<ContactInterface *>(contact->GetFixtureB()->GetBody()->GetUserData());

        if (objA != 0 && objB != 0) {
            objA->BeginContact(objB);
            objB->BeginContact(objA);
        }
    }

    void EndContact(b2Contact * contact) {

        ContactInterface* objA = static_cast<ContactInterface *>(contact->GetFixtureA()->GetBody()->GetUserData());
        ContactInterface* objB = static_cast<ContactInterface *>(contact->GetFixtureB()->GetBody()->GetUserData());

        if (objA != 0 && objB != 0) {
            objA->EndContact(objB);
            objB->EndContact(objA);
        }
    }
};

#endif // COLLISION_HANDLER_H_

