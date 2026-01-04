#pragma once

#include <cmath>
class Interval {
public:
  double Min, Max;

  // Default es empty
  Interval() : Min(+INFINITY), Max(-INFINITY) {}

  Interval(double min, double max) : Min(min), Max(max) {}

  Interval(const Interval &a, const Interval &b) {
    Min = a.Min <= b.Min ? a.Min : b.Min;
    Max = a.Max >= b.Max ? a.Max : b.Max;
  }

  double size() const { return Max - Min; }

  bool contains(double x) const { return Min <= x && Max >= x; }
  bool surrounds(double x) const { return Min < x && Max > x; }

  double clamp(double x) const {
    if (x < Min)
      return Min;
    if (x > Max)
      return Max;
    return x;
  }

  Interval expand(double delta) const {
    auto padding = delta / 2;
    return Interval(Min - padding, Max + padding);
  }

  static const Interval empty, universe;

private:
};

const Interval Interval::empty = Interval(+INFINITY, -INFINITY);
const Interval Interval::universe = Interval(-INFINITY, +INFINITY);
