#ifndef CONTACT_INTERFACE_H_
#define CONTACT_INTERFACE_H_

class ContactInterface {
public:
    virtual ~ContactInterface() {}
    virtual void ProcessImpulse(float impulse) = 0;             // CI_01
    virtual void BeginContact(ContactInterface* object) = 0;    // CI_02
    virtual void EndContact(ContactInterface* object) = 0;      // CI_03
    virtual char Type() = 0;                                    // CI_04
};

#endif // CONTACT_INTERFACE_H_

