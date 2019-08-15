#ifndef UDP_SOCKET_HPP_
#define UDP_SOCKET_HPP_

#include "CommInterface.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <iostream>

struct UdpSettings : public CommSettings {
    bool nonBlockingIo;
    int localPort;
    int remotePort;
    std::string remoteAddress;
};

class UdpSocket : public CommInterface {
public:
    UdpSocket() {
        Defaults();
    }
    ~UdpSocket() {}
    #define HANDLE_ERROR(x) if (x == -1) {std::cerr<<strerror(errno)<<'\n'; return false;}
    bool Init(CommSettings * settings) {
        UdpSettings * s = static_cast<UdpSettings *>(settings);
        if (s == 0) {
            std::cerr << "Udp settings invalid!\n";
            return false;
        }
        LocalPort(s->localPort);
        RemotePort(s->remotePort);
        RemoteAddress(s->remoteAddress);

        int rv;
        rv = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
        HANDLE_ERROR(rv);
        sock_ = rv;

        // Non-blocking I/O
        if (s->nonBlockingIo) {
            int flags = fcntl(sock_, F_GETFL, 0);
            rv = fcntl(sock_, F_SETFL, flags | O_NONBLOCK);
            HANDLE_ERROR(rv);
        }

        if (remotePort_ != -1) {
            memset((char *)&remoteHost_, 0, sizeof(remoteHost_));
            remoteHost_.sin_family = AF_INET;
            remoteHost_.sin_port = htons(remotePort_);
            rv = inet_aton(remoteAddress_.c_str(), &remoteHost_.sin_addr);
            if (rv == 0) {
                std::cerr << "Remote address is not valid!\n";
                return false;
            }
        }

        int optval = 1;
        rv = setsockopt(sock_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
        HANDLE_ERROR(rv);
    
        if (localPort_ != -1) {
            localHost_.sin_family = AF_INET;
            localHost_.sin_port = htons(localPort_);
            localHost_.sin_addr.s_addr = htonl(INADDR_ANY);
            rv = bind(sock_, (sockaddr*)&localHost_, sizeof(localHost_));
            HANDLE_ERROR(rv);
        }

        return true;
    }
    #undef HANDLE_ERROR
    int Read(uint8_t *buffer, int size) {
        int rxSize = -1;
        sockaddr_in remoteAddr;
        socklen_t slen = sizeof(remoteAddr);
        rxSize = recvfrom(sock_, buffer, size, 0, (sockaddr*)&remoteAddr, &slen);
        if (rxSize == -1) {
            if (errno != EAGAIN) {
                std::cerr << "Unable to receive data!" << std::endl;
            }
        }
        return rxSize;
    }
    int Write(uint8_t *buffer, int size) {
        int txSize = -1;
        int slen = sizeof(remoteHost_);
        txSize = sendto(sock_, buffer, size, 0, (const sockaddr *) &remoteHost_, slen);
        if (txSize == -1) {
            std::cerr << "Unable to send data!" << std::endl;
        }
        return txSize;
    }

private:
    void Defaults() {
        ready_ = false;
        sock_ = -1;
        // Default values
        RemoteAddress("127.0.0.1");
        RemotePort(5010);
        LocalPort(5001);
    }
    void RemoteAddress(const std::string & value) { remoteAddress_ = value; }
    void RemotePort(const int value) { remotePort_ = value; }
    void LocalPort(const int value) { localPort_ = value; }

private:
    bool ready_;
    std::string remoteAddress_;
    int remotePort_;
    int localPort_;
    sockaddr_in remoteHost_;
    sockaddr_in localHost_;
    int sock_;
};
    
#endif // UDP_SOCKET_HPP_
