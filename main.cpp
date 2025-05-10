#include <iostream>

constexpr int IMG_WIDTH = 256;
constexpr int IMG_HEIGHT = 256;

int main() {
  std::cout << "P3\n" << IMG_WIDTH << ' ' << IMG_HEIGHT << "\n255\n";

  for (int i = 0; i < IMG_HEIGHT; ++i) {
    std::clog << "\rScanlines remaining: " << (IMG_HEIGHT - i) << ' '
              << std::flush;
    for (int j = 0; j < IMG_WIDTH; ++j) {
      auto r = (double)i / (IMG_WIDTH - 1);
      auto g = (double)j / (IMG_WIDTH - 1);
      auto b = 0;

      int ir = int(255.999 * r);
      int ig = int(255.999 * g);
      int ib = int(255.999 * b);

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }

  std::clog << "Done!.                      \n";
  return 0;
}
