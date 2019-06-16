#ifndef STAR_INTERFACE_H_
#define STAR_INTERFACE_H_

class StarInterface {
public:
    virtual ~StarInterface() {}
    virtual void GetPosition(double & x, double & y) = 0;
};

#endif // STAR_INTERFACE_H_
