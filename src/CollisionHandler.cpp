#include "CollisionHandler.h"

#include "SpaceShip.h"

CollisionHandler::CollisionHandler() {}
CollisionHandler::~CollisionHandler() {
    puts("~CollisionHandler()");
}

void CollisionHandler::PostSolve(b2Contact * contact, const b2ContactImpulse* impulse) {

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

void CollisionHandler::BeginContact(b2Contact * contact) {

    ContactInterface* objA = static_cast<ContactInterface *>(contact->GetFixtureA()->GetBody()->GetUserData());
    ContactInterface* objB = static_cast<ContactInterface *>(contact->GetFixtureB()->GetBody()->GetUserData());

    if (objA != 0 && objB != 0) {
        objA->BeginContact(objB);
        objB->BeginContact(objA);
    }
}

void CollisionHandler::EndContact(b2Contact * contact) {

    ContactInterface* objA = static_cast<ContactInterface *>(contact->GetFixtureA()->GetBody()->GetUserData());
    ContactInterface* objB = static_cast<ContactInterface *>(contact->GetFixtureB()->GetBody()->GetUserData());

    if (objA != 0 && objB != 0) {
        objA->EndContact(objB);
        objB->EndContact(objA);
    }
}
