// Copyright [2019] A.Bekkine
#ifndef PARAMETER_CONTROL_HPP_
#define PARAMETER_CONTROL_HPP_

#include "UniverseParameters.hpp"
#include "CommInterface.h"
#include "CommFactory.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include <GL/glut.h>

class ParameterControl {
public:
    ParameterControl() {
    }
    ~ParameterControl() {}
    void SetParameters(UniverseParameters * params) {
        params_ = params;
    }
    void Init() {
        sock_ = CommFactory::getUdpSocket(3201, -1, "");  
    }

    void Update() {
        if (sock_->Read((uint8_t *)&(params_->rawParams), sizeof(Params)) > 0) {
            params_->Update();
        }
    }

private:
    UniverseParameters * params_;
    CommInterface * sock_;
};

#endif // PARAMETER_CONTROL_HPP_
