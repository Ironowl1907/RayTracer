#pragma once

#include "Color.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "random.h"
#include "vec3.h"
#include <atomic>
#include <cmath>
#include <iostream>
#include <ostream>
#include <thread>

#define PI 3.14159

class Camera {
public:
  double AspectRatio = 16.0 / 9.0;
  int ImageWidth = 800;
  int SamplesPerPixel = 10;
  int MaxDepth = 10;

  double VFov = 90;
  Point3 LookFrom = Point3(0, 0, 0);
  Point3 LookAt = Point3(0, 0, -1);
  Vec3 VUp = Vec3(0, 1, 0);

  double DefocusAngle = 0;
  double FocusDist = 10;

  void render(const Hittable &world) {
    initialize();

    int nCores = std::thread::hardware_concurrency();
    if (nCores <= 0)
      nCores = 1;

    int rowsPerThread = m_imageHeight / nCores;
    int remainder = m_imageHeight % nCores;

    std::vector<std::thread> threads;

    auto renderRows = [this, &world](int from, int to) {
      for (int j = from; j < to; ++j) {
        for (int i = 0; i < ImageWidth; ++i) {
          Color pixelColor(0, 0, 0);

          for (int sample = 0; sample < SamplesPerPixel; ++sample) {
            Ray r = getRay(i, j);
            pixelColor += rayColor(r, MaxDepth, world);
          }

          m_framebuffer[j * ImageWidth + i] = m_pixelSamplesScale * pixelColor;
        }

        m_progressCounter.fetch_add(1);
      }
    };

    int rowStart = 0;
    for (int i = 0; i < nCores; ++i) {
      int rowEnd = rowStart + rowsPerThread;
      if (i == nCores - 1) {
        rowEnd += remainder;
      }

      threads.emplace_back(renderRows, rowStart, rowEnd);
      rowStart = rowEnd;
    }

    while (m_progressCounter < m_imageHeight) {

      std::clog << "\rProgress: " << m_progressCounter.load() << " / "
                << m_imageHeight << std::flush;

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (auto &t : threads) {
      t.join();
    }

    std::clog << "Done.\n";
    writeToImage();
  }

  void writeToImage() const {
    std::cout << "P3\n" << ImageWidth << ' ' << m_imageHeight << "\n255\n";
    for (int j = 0; j < m_imageHeight; ++j) {
      for (int i = 0; i < ImageWidth; ++i) {
        writeColor(std::cout, m_framebuffer[j * ImageWidth + i]);
      }
    }
  }

private:
  void initialize() {
    m_imageHeight = int(ImageWidth / AspectRatio);
    m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

    m_pixelSamplesScale = 1.0 / SamplesPerPixel;

    auto theta = VFov * PI / 180;
    auto h = std::tan(theta / 2);
    auto viewportHeight = 2 * h * FocusDist;
    auto viewportWidth = viewportHeight * (double(ImageWidth) / m_imageHeight);
    m_center = LookFrom;

    m_w = unitVector(LookFrom - LookAt);
    m_u = unitVector(cross(VUp, m_w));
    m_v = cross(m_w, m_u);

    auto viewportU = viewportWidth * m_u;
    auto viewportV = viewportHeight * -m_v;

    m_pixelDeltaU = viewportU / ImageWidth;
    m_pixelDeltaV = viewportV / m_imageHeight;

    auto viewportUpperLeft =
        m_center - (FocusDist * m_w) - viewportU / 2 - viewportV / 2;
    m_pixel00Loc = viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    auto defocus_radius = FocusDist * std::tan((DefocusAngle / 2) * PI / 180);
    m_defocusDiskU = m_u * defocus_radius;
    m_defocusDiskV = m_v * defocus_radius;

    m_framebuffer.resize(m_imageHeight * ImageWidth);
    m_progressCounter.store(0);
  }

  Ray getRay(int i, int j) const {
    auto offset = sampleSquare();
    auto pixel_sample = m_pixel00Loc + ((i + offset.x()) * m_pixelDeltaU) +
                        ((j + offset.y()) * m_pixelDeltaV);

    auto rayOrigin = (DefocusAngle <= 0) ? m_center : defocusDiskSample();
    auto rayDirection = pixel_sample - rayOrigin;
    auto rayTime = Random::double_t();

    return Ray(rayOrigin, rayDirection, rayTime);
  }

  Vec3 sampleSquare() const {
    return Vec3(Random::double_t() - 0.5, Random::double_t() - 0.5, 0);
  }

  Point3 defocusDiskSample() const {
    auto p = randomInUnitDisk();
    return m_center + (p[0] * m_defocusDiskU) + (p[1] * m_defocusDiskV);
  }

  Color rayColor(const Ray &r, const int depth, const Hittable &world) const {
    if (depth <= 0) {
      return Color(0, 0, 0);
    }

    HitRecord rec;
    if (world.hit(r, Interval(0.001, INFINITY), rec)) {
      Ray scattered;
      Color attenuation;
      if (rec.Mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * rayColor(scattered, depth - 1, world);
      }
      return Color(0, 0, 0);
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
  Vec3 m_u, m_v, m_w;
  Vec3 m_defocusDiskU;
  Vec3 m_defocusDiskV;

  std::vector<Color> m_framebuffer;

  std::atomic<int> m_progressCounter;
};
