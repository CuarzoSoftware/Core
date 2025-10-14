#include "CZEase.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace CZ {

// ===================================================================================
// Sine Easing
// ===================================================================================

Float64 CZEase::inSine(Float64 t) noexcept {
  return 1.0 - std::cos(t * M_PI / 2.0);
}

Float64 CZEase::outSine(Float64 t) noexcept { return std::sin(t * M_PI / 2.0); }

Float64 CZEase::inOutSine(Float64 t) noexcept {
  return -0.5 * (std::cos(M_PI * t) - 1.0);
}

// ===================================================================================
// Exponential Easing
// ===================================================================================

Float64 CZEase::inExpo(Float64 t) noexcept {
  if (t <= 0.0)
    return 0.0;
  return std::pow(2.0, 10.0 * (t - 1.0));
}

Float64 CZEase::outExpo(Float64 t) noexcept {
  if (t >= 1.0)
    return 1.0;
  return 1.0 - std::pow(2.0, -10.0 * t);
}

Float64 CZEase::inOutExpo(Float64 t) noexcept {
  if (t <= 0.0)
    return 0.0;
  if (t >= 1.0)
    return 1.0;
  t *= 2.0;
  if (t < 1.0)
    return 0.5 * std::pow(2.0, 10.0 * (t - 1.0));
  return 0.5 * (2.0 - std::pow(2.0, -10.0 * (t - 1.0)));
}

// ===================================================================================
// Circular Easing
// ===================================================================================

Float64 CZEase::inCirc(Float64 t) noexcept {
  return -(std::sqrt(1.0 - t * t) - 1.0);
}

Float64 CZEase::outCirc(Float64 t) noexcept {
  t -= 1.0;
  return std::sqrt(1.0 - t * t);
}

Float64 CZEase::inOutCirc(Float64 t) noexcept {
  t *= 2.0;
  if (t < 1.0)
    return -0.5 * (std::sqrt(1.0 - t * t) - 1.0);
  t -= 2.0;
  return 0.5 * (std::sqrt(1.0 - t * t) + 1.0);
}

// ===================================================================================
// Elastic Easing
// ===================================================================================

Float64 CZEase::inElastic(Float64 t, Float64 a, Float64 p) noexcept {
  if (t == 0.0)
    return 0.0;
  if (t == 1.0)
    return 1.0;
  Float64 s;
  if (a < 1.0) {
    a = 1.0;
    s = p / 4.0;
  } else
    s = p / (2.0 * M_PI) * std::asin(1.0 / a);
  t -= 1.0;
  return -(a * std::pow(2.0, 10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p));
}

Float64 CZEase::outElastic(Float64 t, Float64 a, Float64 p) noexcept {
  if (t == 0.0)
    return 0.0;
  if (t == 1.0)
    return 1.0;
  Float64 s;
  if (a < 1.0) {
    a = 1.0;
    s = p / 4.0;
  } else
    s = p / (2.0 * M_PI) * std::asin(1.0 / a);
  return a * std::pow(2.0, -10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p) +
         1.0;
}

Float64 CZEase::inOutElastic(Float64 t, Float64 a, Float64 p) noexcept {
  if (t == 0.0)
    return 0.0;
  if (t == 1.0)
    return 1.0;
  Float64 s;
  if (a < 1.0) {
    a = 1.0;
    s = p / 4.0;
  } else
    s = p / (2.0 * M_PI) * std::asin(1.0 / a);
  t *= 2.0;
  if (t < 1.0) {
    t -= 1.0;
    return -0.5 *
           (a * std::pow(2.0, 10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p));
  }
  t -= 1.0;
  return a * std::pow(2.0, -10.0 * t) * std::sin((t - s) * (2.0 * M_PI) / p) *
             0.5 +
         1.0;
}

} // namespace CZ
