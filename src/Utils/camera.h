#pragma once

#include "Color.h"
#include "hittable.h"
#include "interval.h"
#include "random.h"
#include "vec3.h"
#include <cmath>
class Camera {
public:
  double AspectRatio = 16.0 / 9.0;
  int ImageWidth = 800;
  int SamplesPerPixel = 10;

  void render(const Hittable &world) {
    initialize();

    std::cout << "P3\n" << ImageWidth << ' ' << m_imageHeight << "\n255\n";

    for (int j = 0; j < m_imageHeight; j++) {
      std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << ' '
                << std::flush;
      for (int i = 0; i < ImageWidth; i++) {
        Color pixelColor(0, 0, 0);
        for (int sample = 0; sample < SamplesPerPixel; ++sample) {
          Ray r = getRay(i, j);
          pixelColor += rayColor(r, world);
        }
        writeColor(std::cout, m_pixelSamplesScale * pixelColor);
      }
    }
    std::clog << "\rDone.                 \n";
  }

private:
  void initialize() {
    m_imageHeight = int(ImageWidth / AspectRatio);
    m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

    m_pixelSamplesScale = 1.0 / SamplesPerPixel;

    auto focalLenght = 1.0;
    auto viewportHeight = 2.0;
    auto viewportWidth = viewportHeight * (double(ImageWidth) / m_imageHeight);
    m_center = Point3(0, 0, 0);

    auto viewportU = Vec3(viewportWidth, 0, 0);
    auto viewportV = Vec3(0, -viewportHeight, 0);

    m_pixelDeltaU = viewportU / ImageWidth;
    m_pixelDeltaV = viewportV / m_imageHeight;

    auto viewportUpperLeft =
        m_center - Vec3(0, 0, focalLenght) - viewportU / 2 - viewportV / 2;
    m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);
  }

  Ray getRay(int i, int j) const {
    auto offset = sampleSquare();
    auto pixel_sample = m_pixel00Loc + ((i + offset.x()) * m_pixelDeltaU) +
                        ((j + offset.y()) * m_pixelDeltaV);

    auto ray_origin = m_center;
    auto ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
  }

  Vec3 sampleSquare() const {
    return Vec3(Random::double_t() - 0.5, Random::double_t() - 0.5, 0);
  }

  Color rayColor(const Ray &r, const Hittable &world) const {
    HitRecord rec;
    if (world.hit(r, Interval(0, INFINITY), rec)) {
      return 0.5 * (rec.Normal + Color(1, 1, 1));
    }

    Vec3 unitDirection = unitVector(r.direction());
    auto a = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - a) * Color(1, 1, 1) + a * Color(0.5, 0.7, 1.0);
  }

private:
  int m_imageHeight;
  double m_pixelSamplesScale;
  Point3 m_center;
  Point3 m_pixel00Loc;
  Vec3 m_pixelDeltaU;
  Vec3 m_pixelDeltaV;
};
