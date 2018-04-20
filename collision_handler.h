#ifndef COLLISION_HANDLER_H_
#define COLLISION_HANDLER_H_

#include <Box2D.h>

#include "space_ship.h"

class CollisionHandler : public b2ContactListener {

    void PostSolve(b2Contact * contact, const b2ContactImpulse* impulse) {

        void * userData = 0;
        userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // Process impulse
                int count = contact->GetManifold()->pointCount;
                float maxImpulse = 0.0;
                for (int i=0; i<count; ++i) {
                    maxImpulse = b2Max(maxImpulse, impulse->normalImpulses[i]);
                }

                ship->ProcessImpulse(maxImpulse);
            }
        }
        else {
            std::cout << "userData is '0', maybe check fixture B?\n";
        }
    }

    void BeginContact(b2Contact * contact) {
        void * userData = 0;

        userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // Process contact begin (fixture A).
                ship->BeginContact();
            }
        }
        else {
            std::cout << "userData is '0', maybe check fixture B?\n";
        }
    }

    void EndContact(b2Contact * contact) {
        void * userData = 0;

        userData = contact->GetFixtureA()->GetBody()->GetUserData();
        if (userData != 0) {
            SpaceShip * ship = static_cast<SpaceShip *>(userData);
            if (ship) {
                // Process contact end (fixture A).
                ship->EndContact();
            }
        }
        else {
            std::cout << "userData is '0', mayba check fixture B?\n";
        }
    }
};

#endif // COLLISION_HANDLER_H_

