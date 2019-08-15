#ifndef COMM_INTERFACE_H
#define COMM_INTERFACE_H

#include <cstdint>

struct CommSettings {
    virtual ~CommSettings() {}
};

class CommInterface {
public:
    virtual ~CommInterface() {}
    virtual bool Init(CommSettings * settings) = 0;
    virtual int Read(uint8_t *buffer, int size) = 0;
    virtual int Write(uint8_t *buffer, int size) = 0;
};

#endif // COMM_INTERFACE_H