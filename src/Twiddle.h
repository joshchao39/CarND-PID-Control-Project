//
// Created by Josh Chao on 6/17/17.
//

#ifndef PID_TWIDDLE_H
#define PID_TWIDDLE_H

#include <vector>
#include <iostream>
#include <math.h>

class Twiddle {
    std::vector<double> K;
    std::vector<double> dK;
    double tolerance;
    int polarity = true;
    size_t pos = 0;

public:
    double bestError = NAN;

    Twiddle(double tolerance);

    std::vector<double> Reset(std::vector<double> K, std::vector<double> dK);

    bool IsToleranceMet();

    std::vector<double> ComputeNewK(double error);
};


#endif //PID_TWIDDLE_H
