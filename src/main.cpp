#include <memory>

#include "Utils/camera.h"
#include "Utils/hittable_list.h"
#include "Utils/sphere.h"
#include "Utils/vec3.h"

using std::make_shared;

int main() {
  HittableList world;

  // World
  auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto material_left = make_shared<Dielectric>(1.5);
  auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
  auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

  world.add(
      make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
  world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
  world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

  Camera cam;
  cam.AspectRatio = 16.0 / 9.0;
  cam.ImageWidth = 400;
  cam.SamplesPerPixel = 100;
  cam.MaxDepth = 100;

  cam.render(world);

  // Render
}
