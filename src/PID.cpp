#include "PID.h"
#include <iostream>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {
  this->i_error = 0;
  this->cte_prev = 0;
}

PID::~PID() {}

void PID::SetK(std::vector<double> K) {
  this->Kp = K[0];
  this->Ki = K[1];
  this->Kd = K[2];
}

double PID::GetSteeringAngle(double cte) {
  double d_error = cte - cte_prev;
  i_error += cte;
  double steering = -Kp * cte - Ki * i_error - Kd * d_error;
  cte_prev = cte;
  return steering;
}

