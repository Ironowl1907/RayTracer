#pragma once

#include <cmath>
class Interval {
public:
  double Min, Max;

  // Default es empty
  Interval() : Min(+INFINITY), Max(-INFINITY) {}

  Interval(double min, double max) : Min(min), Max(max) {}

  double size() const { return Max - Min; }

  bool contains(double x) const { return Min <= x && Max >= x; }
  bool surrounds(double x) const { return Min < x && Max > x; }

  static const Interval empty, universe;

private:
};

const Interval Interval::empty = Interval(+INFINITY, -INFINITY);
const Interval Interval::universe = Interval(-INFINITY, +INFINITY);
