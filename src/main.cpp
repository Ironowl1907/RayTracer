#include <memory>

#include "Utils/Color.h"
#include "Utils/camera.h"
#include "Utils/hittable_list.h"
#include "Utils/material.h"
#include "Utils/sphere.h"
#include "Utils/vec3.h"

using std::make_shared;

auto R = std::cos(PI / 4);

int main() {
  HittableList world;
  auto materialLeft = make_shared<Lambertian>(Color(0, 0, 1));
  auto materialRight = make_shared<Lambertian>(Color(1, 0, 0));

  world.add(make_shared<Sphere>(Point3(-R, 0, -1), R, materialLeft));
  world.add(make_shared<Sphere>(Point3(R, 0, -1), R, materialRight));

  Camera cam;
  cam.AspectRatio = 16.0 / 9.0;
  cam.ImageWidth = 400;
  cam.SamplesPerPixel = 100;
  cam.MaxDepth = 100;
  cam.vFov = 90;

  cam.render(world);

  // Render
}
