#pragma once

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include "random.h"
#include <algorithm>

class BVHNode : public Hittable {
public:
  BVHNode(HittableList list) : BVHNode(list.Objects, 0, list.Objects.size()) {}

  BVHNode(std::vector<shared_ptr<Hittable>> &objects, size_t start,
          size_t end) {
    int axis = Random::int_t(0, 2);

    auto comparator = (axis == 0)   ? boxXCompare
                      : (axis == 1) ? boxYCompare
                                    : boxZCompare;

    size_t objectSpan = end - start;

    if (objectSpan == 1) {
      left = right = objects[start];
    } else if (objectSpan == 2) {
      left = objects[start];
      right = objects[start + 1];
    } else {
      std::sort(std::begin(objects) + start, std::begin(objects) + end,
                comparator);

      auto mid = start + objectSpan / 2;
      left = std::make_shared<BVHNode>(objects, start, mid);
      right = std::make_shared<BVHNode>(objects, mid, end);
    }

    bbox = AABB(left->boundingBox(), right->boundingBox());
  }

  bool hit(const Ray &r, const Interval &rayI, HitRecord &rec) const override {
    if (!bbox.Hit(r, rayI))
      return false;

    bool hit_left = left->hit(r, rayI, rec);
    bool hit_right =
        right->hit(r, Interval(rayI.Min, hit_left ? rec.T : rayI.Max), rec);

    return hit_left || hit_right;
  }

  AABB boundingBox() const override { return bbox; }

private:
  static bool boxCompare(const shared_ptr<Hittable> a,
                         const shared_ptr<Hittable> b, int axis_index) {
    auto a_axis_interval = a->boundingBox().axisInterval(axis_index);
    auto b_axis_interval = b->boundingBox().axisInterval(axis_index);
    return a_axis_interval.Min < b_axis_interval.Min;
  }

  static bool boxXCompare(const shared_ptr<Hittable> a,
                          const shared_ptr<Hittable> b) {
    return boxCompare(a, b, 0);
  }

  static bool boxYCompare(const shared_ptr<Hittable> a,
                          const shared_ptr<Hittable> b) {
    return boxCompare(a, b, 1);
  }

  static bool boxZCompare(const shared_ptr<Hittable> a,
                          const shared_ptr<Hittable> b) {
    return boxCompare(a, b, 2);
  }

private:
  shared_ptr<Hittable> left;
  shared_ptr<Hittable> right;
  AABB bbox;
};
