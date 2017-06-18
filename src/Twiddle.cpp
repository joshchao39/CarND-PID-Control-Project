//
// Created by Josh Chao on 6/17/17.
//

#include "Twiddle.h"

Twiddle::Twiddle(double tolerance) {
  this->tolerance = tolerance;
}

std::vector<double> Twiddle::Reset(std::vector<double> K, std::vector<double> dK) {
  this->K = K;
  this->dK = dK;
  this->bestError = NAN;
  this->polarity = true;
  this->pos = 0;

  return this->K;
}

std::vector<double> Twiddle::ComputeNewK(double error) {
  if (isnan(bestError)) {
    // Has just reset
    bestError = error;
    K[pos] += dK[pos];
    polarity = 1;
  } else if (error < bestError) {
    // Better error found => Larger step; Next parameter
    bestError = error;
    dK[pos] *= 1.1;
    pos = ++pos % K.size();
    K[pos] += dK[pos];
    polarity = 1;
  } else {
    // Try both directions
    if (polarity == 1) {
      K[pos] -= 2 * dK[pos];
      polarity = -1;
    } else if (polarity == -1) {
      K[pos] += dK[pos];
      dK[pos] *= 0.9;
      pos = ++pos % K.size();
      K[pos] += dK[pos];
      polarity = 1;
    }
  }

  return K;
}

bool Twiddle::IsToleranceMet() {
  double sum = 0;
  for (double k: dK) {
    sum += k;
  }
  return sum < tolerance;
}

