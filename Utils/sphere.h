#include "hitable.h"
#include "vec3.h"
#include <cmath>

class sphere : public hitabble {
public:
  sphere(const point3 &center, double radius)
      : m_center(center), m_radius(std::fmax(0, radius)) {}

  bool hit(const ray &r, double ray_tmin, double ray_tmax,
           hit_record &rec) const override {
    vec3 oc = m_center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - m_radius * m_radius;

    auto discriminant = h * h - a * c;

    if (discriminant < 0)
      return false;

    auto sqrtd = std::sqrt(discriminant);

    double root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
      root = (h + sqrtd) / a;
      if (root <= ray_tmin || ray_tmax <= root)
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - m_center) / m_radius;
    rec.set_face_normal(r, outward_normal);

    return true;
  }

private:
private:
  point3 m_center;
  double m_radius;
}
