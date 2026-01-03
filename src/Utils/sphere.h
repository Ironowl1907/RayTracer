#pragma once

#include "hittable.h"
#include "vec3.h"
#include <cmath>
#include <memory>
class Sphere : public Hittable {
public:
  Sphere(const Point3 &staticCenter, double radius,
         std::shared_ptr<Material> mat)
      : m_center(staticCenter, Vec3(0, 0, 0)), m_radius(std::fmax(0, radius)),
        m_mat(mat) {}

  Sphere(const Point3 &center1, const Point3 &center2, double radius,
         std::shared_ptr<Material> mat)
      : m_center(center1, center2 - center1), m_radius(std::fmax(0, radius)),
        m_mat(mat) {}

  virtual bool hit(const Ray &r, const Interval &rayI,
                   HitRecord &rec) const override {

    Point3 currentCenter = m_center.at(r.time());
    Vec3 oc = currentCenter - r.origin();
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
    Vec3 outwardNormal = (rec.P - currentCenter) / m_radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.Mat = m_mat;

    return true;
  }

private:
private:
  Ray m_center;
  double m_radius;
  std::shared_ptr<Material> m_mat;
};
