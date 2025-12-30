#pragma once

#include "interval.h"
#include "vec3.h"
#include <iostream>
#include <ostream>

using Color = Vec3;

inline double linear2Gamma(double linearComponent) {
  if (linearComponent > 0)
    return std::sqrt(linearComponent);
  return 0;
}

inline void writeColor(std::ostream &out, const Color &pixel_color) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  r = linear2Gamma(r);
  g = linear2Gamma(g);
  b = linear2Gamma(b);

  static const Interval intensity(0.000, 0.999);
  int rbyte = int(256 * intensity.clamp(r));
  int gbyte = int(256 * intensity.clamp(g));
  int bbyte = int(256 * intensity.clamp(b));

  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
