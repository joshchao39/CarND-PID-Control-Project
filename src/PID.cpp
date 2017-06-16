#include "PID.h"
#include <iostream>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->i_error = 0;
    this->cte_prev = 0;
}

double PID::GetSteeringAngle(double cte) {
    double d_error = cte - cte_prev;
    i_error += cte;
    double steering = -Kp * cte - Ki * i_error - Kd * d_error;
    cte_prev = cte;
    return steering;
}

