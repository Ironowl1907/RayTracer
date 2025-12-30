#pragma once

#include "Color.h"
#include "Ray.h"
#include "hittable.h"
#include "vec3.h"
class Material {
public:
  virtual ~Material() = default;

  virtual bool scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation,
                       Ray &scattered) const {
    return false;
  }
};

class Lambertian : public Material {
public:
  Lambertian(const Color &albedo) : m_albedo(albedo) {}

  bool scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation,
               Ray &scattered) const override {
    auto scatterDirection = rec.Normal + randomUnitVector();

    // Catch degenrate scatter direction
    if (scatterDirection.nearZero()) {
      scatterDirection = rec.Normal;
    }

    scattered = Ray(rec.P, scatterDirection);
    attenuation = m_albedo;
    return true;
  }

private:
  Color m_albedo;
};

class Metal : public Material {
public:
  Metal(const Color &albedo, const double &fuzz)
      : m_albedo(albedo), m_fuzz(fuzz) {}

  bool scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation,
               Ray &scattered) const override {
    Vec3 reflected = reflect(rIn.direction(), rec.Normal);
    reflected = unitVector(reflected) + (m_fuzz * randomUnitVector());
    scattered = Ray(rec.P, reflected);
    attenuation = m_albedo;
    return (dot(scattered.direction(), rec.Normal) > 0);
  }

private:
  Color m_albedo;
  double m_fuzz;
};
