#include <iostream>

#include "Utils/Color.h"
#include "Utils/Ray.h"
#include "Utils/vec3.h"

Color rayColor(const Ray &r) {
  Vec3 unit_direction = unit_vector(r.direction());
  auto a = 0.5 * (unit_direction.y() + 1.0);
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
  auto viewport_upper_left =
      cameraCenter - Vec3(0, 0, focalLenght) - viewportU / 2 - viewportV / 2;
  auto pixel00_loc = viewport_upper_left + 0.5 * (pixelDeltaU + pixelDeltaV);

  // Render
  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (int j = 0; j < imageHeight; j++) {
    std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' '
              << std::flush;
    for (int i = 0; i < imageWidth; i++) {
      auto pixel_center = pixel00_loc + (i * pixelDeltaU) + (j * pixelDeltaV);
      auto ray_direction = pixel_center - cameraCenter;
      Ray r(cameraCenter, ray_direction);

      Color pixelColor = rayColor(r);
      write_color(std::cout, pixelColor);
    }
  }
  std::clog << "\rDone.                 \n";
}
