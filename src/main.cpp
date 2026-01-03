#include <memory>

#include "Utils/Color.h"
#include "Utils/camera.h"
#include "Utils/hittable_list.h"
#include "Utils/material.h"
#include "Utils/random.h"
#include "Utils/sphere.h"
#include "Utils/vec3.h"

using std::make_shared;

auto R = std::cos(PI / 4);

int main() {
  HittableList world;

  auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = Random::double_t();
      Point3 center(a + 0.9 * Random::double_t(), 0.2,
                    b + 0.9 * Random::double_t());

      if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<Material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Color::random() * Color::random();
          sphere_material = make_shared<Lambertian>(albedo);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Color::random(0.5, 1);
          auto fuzz = Random::double_t(0, 0.5);
          sphere_material = make_shared<Metal>(albedo, fuzz);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<Dielectric>(1.5);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<Dielectric>(1.5);
  world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  Camera cam;

  cam.AspectRatio = 16.0 / 9.0;
  cam.ImageWidth = 800;
  cam.SamplesPerPixel = 5;
  cam.MaxDepth = 50;

  cam.VFov = 20;
  cam.LookFrom = Point3(13, 2, 3);
  cam.LookAt = Point3(0, 0, 0);
  cam.VUp = Vec3(0, 1, 0);

  cam.DefocusAngle = 0.6;
  cam.FocusDist = 10.0;

  cam.render(world);
}
