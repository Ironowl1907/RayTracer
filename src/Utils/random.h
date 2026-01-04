#pragma once

#include <cmath>
#include <cstdlib>
#include <random>

class Random {
public:
  static inline double double_t() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
  }
  static inline double double_t(double min, double max) {
    return min + (max - min) * double_t();
  }

  static inline int int_t(int min, int max) {
    return (int)double_t(min, max + 1);
  }
};
