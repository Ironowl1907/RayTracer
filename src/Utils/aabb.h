#pragma once

#include "Ray.h"
#include "interval.h"
#include "vec3.h"

class AABB {
public:
  Interval X, Y, Z;

  AABB() {}
  AABB(const Interval &x, const Interval &y, const Interval &z)
      : X(x), Y(y), Z(z) {}

  AABB(const Point3 &a, const Point3 &b) {
    X = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
    Y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
    Z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
  }

  AABB(const AABB &box0, const AABB &box1) {
    X = Interval(box0.X, box1.X);
    Y = Interval(box0.Y, box1.Y);
    Z = Interval(box0.Z, box1.Z);
  }

  const Interval &axisInterval(int n) const {
    if (n == 1)
      return Y;
    if (n == 2)
      return Z;
    return X;
  }

  bool Hit(const Ray &r, Interval rayT) const {
    const Point3 &rayOrig = r.origin();
    const Vec3 &rayDir = r.direction();

    for (int axis = 0; axis < 3; axis++) {
      const Interval &ax = axisInterval(axis);
      const double adinv = 1.0 / rayDir[axis];

      auto t0 = (ax.Min - rayOrig[axis]) * adinv;
      auto t1 = (ax.Max - rayOrig[axis]) * adinv;

      if (t0 < t1) {
        if (rayT.Min < t0)
          rayT.Min = t0;
        if (t1 < rayT.Max)
          rayT.Max = t1;
      } else {
        if (t1 > rayT.Min)
          rayT.Min = t1;
        if (t0 < rayT.Max)
          rayT.Max = t0;
      }

      if (rayT.Max <= rayT.Min)
        return false;
    }
    return true;
  }

  int longestAxis() const {
    if (X.size() > Y.size())
      return X.size() > Z.size() ? 0 : 2;
    return Y.size() > Z.size() ? 1 : 2;
  }

  static const AABB empty, universe;

private:
private:
};

const AABB AABB::empty =
    AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe =
    AABB(Interval::universe, Interval::universe, Interval::universe);
