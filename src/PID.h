#ifndef PID_H
#define PID_H

#include <vector>

class PID {
public:
  /*
  * Errors
  */
  double i_error;
  double cte_prev;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void SetK(std::vector<double> K);

  /*
  * Update the PID error variables given cross track error.
  */
  double GetSteeringAngle(double cte);
};

#endif /* PID_H */
