#pragma once

#include "Color.h"
#include "Ray.h"
#include "hittable.h"
#include "random.h"
#include "vec3.h"
#include <cmath>
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

class Dielectric : public Material {
public:
  Dielectric(const double &refractionIndex)
      : m_refractionIndex(refractionIndex) {}

  bool scatter(const Ray &rIn, const HitRecord &rec, Color &attenuation,
               Ray &scattered) const override {
    attenuation = Color(1, 1, 1);
    double ri = rec.FrontFace ? (1.0 / m_refractionIndex) : m_refractionIndex;

    Vec3 unitDirection = unitVector(rIn.direction());
    double cosTheta = std::fmin(dot(-unitDirection, rec.Normal), 1.0);
    double sintTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = ri * sintTheta > 1.0;
    Vec3 direction;
    if (cannotRefract || reflectance(cosTheta, ri) > Random::double_t()) {
      direction = reflect(unitDirection, rec.Normal);
    } else {
      direction = refract(unitDirection, rec.Normal, ri);
    }

    scattered = Ray(rec.P, direction);

    return true;
  }

private:
  static double reflectance(double cosine, double refractionIndex) {
    auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
  }

private:
  double m_refractionIndex;
};
