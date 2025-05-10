#include <iostream>

#include "Utils/Color.h"
#include "Utils/vec3.h"

constexpr int IMG_WIDTH = 256;
constexpr int IMG_HEIGHT = 256;

int main() {
  std::cout << "P3\n" << IMG_WIDTH << ' ' << IMG_HEIGHT << "\n255\n";

  for (int i = 0; i < IMG_HEIGHT; ++i) {
    std::clog << "\rScanlines remaining: " << (IMG_HEIGHT - i) << ' '
              << std::flush;
    for (int j = 0; j < IMG_WIDTH; ++j) {
      auto pixel_color =
          color(double(i) / (IMG_WIDTH - 1), double(j) / (IMG_HEIGHT - 1), 0);
      write_color(std::cout, pixel_color);
    }
  }

  std::clog << "Done!.                      \n";
  return 0;
}
