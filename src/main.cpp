#include <memory>

#include "Utils/camera.h"
#include "Utils/hittable_list.h"
#include "Utils/sphere.h"
#include "Utils/vec3.h"

int main() {
  HittableList world;

  // World
  world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  Camera cam;
  cam.AspectRatio = 16.0 / 9.0;
  cam.ImageWidth = 400;

  cam.render(world);

  // Render
}
