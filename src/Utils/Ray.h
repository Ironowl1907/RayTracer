#pragma once

#include "vec3.h"

class Ray {
public:
  Ray() {};

  Ray(const Point3 &origin, const Vec3 &direction)
      : m_orig(origin), m_dir(direction), m_time(0) {}

  Ray(const Point3 &origin, const Vec3 &direction, double time)
      : m_orig(origin), m_dir(direction), m_time(time) {}

  const Point3 &origin() const { return m_orig; }
  const Vec3 &direction() const { return m_dir; }
  const double &time() const { return m_time; }

  Point3 at(double t) const { return m_orig + t * m_dir; }

private:
  Point3 m_orig;
  Vec3 m_dir;
  double m_time;
};
