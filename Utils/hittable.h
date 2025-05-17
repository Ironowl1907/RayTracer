#pragma once
#include "Ray.h"
#include "vec3.h"

struct hit_record {
  point3 p;
  double t;
  vec3 normal;
  bool front_face;

  void set_face_normal(const ray &r, const vec3 &outward_normal) {
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.

    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual ~hittable() = default;

  virtual bool hit(const ray &r, double ray_tmin, double ray_tmax,
                   hit_record &rec) const = 0;
};
