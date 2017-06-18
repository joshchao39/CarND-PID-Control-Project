//
// Created by Josh Chao on 6/17/17.
//

#include "Twiddle.h"
#include <math.h>

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

//std::vector<double> Twiddle::ComputeNewK2(double error) {
//  if (isnan(bestError)) {
//    // Has just reset
//    bestError = error;
//    K[pos] += dK[pos];
//  } else if (error < bestError) {
//    // Better error found => Larger step; Next parameter
//    bestError = error;
//    dK[pos] *= 1.1;
//    polarity = true;
//    pos++;
//    K[pos] += dK[pos];
//  } else {
//    if (polarity) {
//      // Worse error found => Try the other direction
//      K[pos] -= 2 * dK[pos];
//      polarity = false;
//    } else {
//      // Worse error found and both direction has been tried => Smaller step; Next parameter
//      K[pos] += dK[pos];
//      polarity = true;
//      dK[pos] *= 0.9;
//      pos++;
//    }
//  }
//
//  if (pos == K.size()) {
//    pos -= K.size();
//  }
//
//  return K;
//}

bool Twiddle::IsToleranceMet() {
  double sum = 0;
  for (double k: dK) {
    sum += k;
  }
  return sum < tolerance;
}

