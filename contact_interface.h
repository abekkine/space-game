#ifndef CONTACT_INTERFACE_H_
#define CONTACT_INTERFACE_H_

class ContactInterface {
public:
    virtual ~ContactInterface() {}
    virtual void ProcessImpulse(float impulse) = 0;
    virtual void BeginContact(ContactInterface* object) = 0;
    virtual void EndContact(ContactInterface* object) = 0;
};

#endif // CONTACT_INTERFACE_H_

