#ifndef COMM_FACTORY_HPP_
#define COMM_FACTORY_HPP_

#include "CommInterface.h"
#include "UdpSocket.hpp"
#include "SharedMemory.hpp"

class CommFactory {
public:
    static CommInterface * getUdpSocket(const int localPort, const int remotePort, const std::string & remoteHost) {
        UdpSettings s;
        s.nonBlockingIo = true;
        s.localPort = localPort;
        s.remotePort = remotePort;
        s.remoteAddress = remoteHost;
        CommInterface * com = new UdpSocket();
        if (com->Init(&s)) {
            return com;
        }
        return 0;
    }
    static CommInterface * getSharedMemory(const std::string name, int size) {
        SharedMemSettings s;
        s.name = name;
        s.size = size;
        CommInterface * com = new SharedMemory();
        if (com->Init(&s)) {
            return com;
        }
        return 0;
    }
};

#endif // COMM_FACTORY_HPP_
