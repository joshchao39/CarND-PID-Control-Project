//
// Created by Josh Chao on 6/17/17.
//

#ifndef PID_TWIDDLE_H
#define PID_TWIDDLE_H

#include <vector>
#include <iostream>

class Twiddle {
    std::vector<double> K;
    std::vector<double> dK;
    double tolerance;
    double bestError;
    int polarity;
    size_t pos;

public:
    Twiddle(double tolerance);

    std::vector<double> Reset(std::vector<double> K, std::vector<double> dK);

    bool IsToleranceMet();

    std::vector<double> ComputeNewK(double error);
};


#endif //PID_TWIDDLE_H
