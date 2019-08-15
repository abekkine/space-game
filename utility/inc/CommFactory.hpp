#ifndef COMM_FACTORY_HPP_
#define COMM_FACTORY_HPP_

#include "CommInterface.h"
#include "UdpSocket.hpp"

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
        } else {
            return 0;
        }
    }
};

#endif // COMM_FACTORY_HPP_
