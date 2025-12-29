#include <iostream>

#include "Utils/Color.h"
#include "Utils/Ray.h"
#include "Utils/vec3.h"

double hitSphere(const Point3 &center, double radius, const Ray &r) {
  Vec3 oc = center - r.origin();
  auto a = r.direction().length_squared();
  auto h = dot(r.direction(), oc);
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = h * h - a * c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    return (h - std::sqrt(discriminant)) / a;
  }
}

Color rayColor(const Ray &r) {
  auto t = hitSphere(Point3(0.0, 0.0, -1.0), 0.5, r);
  if (t > 0.0) {
    Vec3 n = unitVector(r.at(t) - Vec3(0.0, 0.0, -1.0));
    return 0.5 * Color(n.x() + 1, n.y() + 1, n.z() + 1);
  }
  Vec3 unitDirection = unitVector(r.direction());
  auto a = 0.5 * (unitDirection.y() + 1.0);
  return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {

  // Image
  auto aspectRatio = 16.0 / 9.0;
  int imageWidth = 400;

  int imageHeight = int(imageWidth / aspectRatio);
  imageHeight = (imageHeight < 1) ? 1 : imageHeight;

  // Camera
  auto focalLenght = 1.0;
  auto viewportHeight = 2.0;
  auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
  auto cameraCenter = Point3(0, 0, 0);

  auto viewportU = Vec3(viewportWidth, 0, 0);
  auto viewportV = Vec3(0, -viewportHeight, 0);

  auto pixelDeltaU = viewportU / imageWidth;
  auto pixelDeltaV = viewportV / imageHeight;

  // Calculate the location of the upper left pixel.
  auto viewportUpperLeft =
      cameraCenter - Vec3(0, 0, focalLenght) - viewportU / 2 - viewportV / 2;
  auto pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

  // Render
  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (int j = 0; j < imageHeight; j++) {
    std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' '
              << std::flush;
    for (int i = 0; i < imageWidth; i++) {
      auto pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
      auto rayDirection = pixelCenter - cameraCenter;
      Ray r(cameraCenter, rayDirection);

      Color pixelColor = rayColor(r);
      writeColor(std::cout, pixelColor);
    }
  }
  std::clog << "\rDone.                 \n";
}
