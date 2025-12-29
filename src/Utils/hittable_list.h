#pragma once

#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::unique_ptr;

class HittableList : public Hittable {
public:
  std::vector<shared_ptr<Hittable>> Objects;

  HittableList() {}
  HittableList(shared_ptr<Hittable> object) { add(object); }

  void clear() { Objects.clear(); }

  void add(shared_ptr<Hittable> object) { Objects.push_back(object); }

  virtual bool hit(const Ray &r, const Interval & rayI,
                   HitRecord &rec) const override {
    HitRecord tempRec;
    bool hitAnything = false;
    auto closeesSoFar = rayI.Max;

    for (const auto &object : Objects) {
      if (object->hit(r, Interval(rayI.Min, closeesSoFar), tempRec)) {
        hitAnything = true;
        closeesSoFar = tempRec.T;
        rec = tempRec;
      }
    }

    return hitAnything;
  }
};
