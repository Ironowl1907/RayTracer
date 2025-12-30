#pragma once

#include "Color.h"
#include "hittable.h"
#include "interval.h"
#include "vec3.h"
#include <cmath>
class Camera {
public:
  double AspectRatio = 16.0 / 9.0;
  int ImageWidth = 800;

  void render(const Hittable &world) {
    initialize();

    std::cout << "P3\n" << ImageWidth << ' ' << m_imageHeight << "\n255\n";

    for (int j = 0; j < m_imageHeight; j++) {
      std::clog << "\rScanlines remaining: " << (m_imageHeight - j) << ' '
                << std::flush;
      for (int i = 0; i < ImageWidth; i++) {
        auto pixelCenter =
            m_pixel00Loc + (i * m_pixelDeltaU) + (j * m_pixelDeltaV);
        auto rayDirection = pixelCenter - m_center;
        Ray r(m_center, rayDirection);

        Color pixelColor = rayColor(r, world);
        writeColor(std::cout, pixelColor);
      }
    }
    std::clog << "\rDone.                 \n";
  }

private:
  void initialize() {
    m_imageHeight = int(ImageWidth / AspectRatio);
    m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

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
  Point3 m_center;
  Point3 m_pixel00Loc;
  Vec3 m_pixelDeltaU;
  Vec3 m_pixelDeltaV;
};
