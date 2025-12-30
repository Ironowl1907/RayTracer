#pragma once

#include "hittable.h"
#include "vec3.h"
#include <cmath>
#include <memory>
class Sphere : public Hittable {
public:
  Sphere(const Point3 &center, const double radius,
         std::shared_ptr<Material> mat)
      : m_center(center), m_radius(std::fmax(0, radius)), m_mat(mat) {
    // TODO: Initialize the material pointer
  }

  virtual bool hit(const Ray &r, const Interval &rayI,
                   HitRecord &rec) const override {

    Vec3 oc = m_center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - m_radius * m_radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
      return false;

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!rayI.surrounds(root)) {
      root = (h + sqrtd) / a;

      if (!rayI.surrounds(root)) {
        return false;
      }
    }

    rec.T = root;
    rec.P = r.at(rec.T);
    Vec3 outwardNormal = (rec.P - m_center) / m_radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.Mat = m_mat;

    return true;
  }

private:
private:
  Point3 m_center;
  double m_radius;
  std::shared_ptr<Material> m_mat;
};
