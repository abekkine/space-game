#ifndef SHARED_MEMORY_HPP
#define SHARED_MEMORY_HPP

#include "CommInterface.h"

#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <string>

struct SharedMemSettings : public CommSettings {
    std::string name;
    int size;
};

class SharedMemory : public CommInterface {
public:
    SharedMemory() {
        Defaults();
    }
    ~SharedMemory() {
        Detach();
    }
    bool Init(CommSettings * settings) {
        SharedMemSettings * s = static_cast<SharedMemSettings *>(settings);
        if (s == 0) {
            return false;
        }
        name_ = s->name;
        size_ = s->size;

        if (Connect() == false) {
            if (Create() == false) {
                return false;
            }
        }

        if (Attach() == false) {
            return false;
        }

        return true;
    }
    int Read(uint8_t * buffer, int size) {
        int rxSize;
        if (size <= 0) {
            rxSize = size;
        } else {
            rxSize = std::min<int>(size_, size);
            memcpy(buffer, memory_, rxSize);
        }
        return rxSize;
    }
    int Write(uint8_t * buffer, int size) {
        int txSize;
        if (size <= 0) {
            txSize = size;
        } else {
            txSize = std::min<int>(size_, size);
            memcpy(memory_, buffer, txSize);
        }
        return txSize;
    }

private:
    void Defaults() {
        name_ = "NoName";
        size_ = 1024;
    }
    bool Connect() {
        fd_ = shm_open(name_.c_str(), O_RDWR, S_IRUSR|S_IWUSR);
        if (fd_ > 0) {
            return true;
        }
        return false;
    }
    bool Create() {
        fd_ = shm_open(name_.c_str(), O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
        if (fd_ > 0) {
            if (ftruncate(fd_, size_) == 0) {
                return true;
            }
        }
        return false;
    }
    bool Attach() {
        memory_ = (uint8_t *) mmap(NULL, size_, PROT_READ|PROT_WRITE, MAP_SHARED, fd_, 0);
        if (memory_ != 0) {
            return true;
        }
        return false;
    }
    void Detach() {
        if (munmap(memory_, size_) != 0) {
            std::cerr << strerror(errno) << '\n';
        } else {
            close(fd_);
        }
    }

private:
    std::string name_;
    int size_;
    uint8_t * memory_;
    int fd_;
};

#endif // SHARED_MEMORY_HPP
