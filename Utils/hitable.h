#pragma once

#include "vec3.h"
struct hit_record {
  point3 p;
  double t;
  vec3 normal;
};

class hitabble {
public:
  virtual ~hitabble() = default;

  virtual bool hit(const ray &r, double ray_tmin, double ray_tmax,
                   hit_record &rec) const = 0;
};
