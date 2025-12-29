#pragma once

#include "Ray.h"
#include "interval.h"
#include "vec3.h"

class HitRecord {
public:
  Point3 P;
  Vec3 Normal;
  double T;
  bool FrontFace;

  void setFaceNormal(const Ray &r, const Vec3 &outwardNormal) {
    FrontFace = dot(r.direction(), outwardNormal) < 0;
    Normal = FrontFace ? outwardNormal : -outwardNormal;
  }
};

class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool hit(const Ray &r, const Interval & rayI,
                   HitRecord &rec) const = 0;
};
