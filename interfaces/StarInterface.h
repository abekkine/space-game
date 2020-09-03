#ifndef STAR_INTERFACE_H_
#define STAR_INTERFACE_H_

#include <string>

class StarInterface {
public:
    virtual ~StarInterface() {}
    virtual void GetPosition(double & vx, double & vy) = 0;
    virtual float * GetColor() = 0;
    virtual float GetRadius() = 0;
    virtual std::string & GetName() = 0;
    virtual int GetType() = 0;
    virtual float GetMass() = 0;
};

#endif // STAR_INTERFACE_H_
