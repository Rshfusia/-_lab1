#include "geometry_tasks.hpp"
#include <cmath>
#include <stdexcept>

template <std::floating_point T>
T calculate_triangle_area(const Vector<T>& a, const Vector<T>& b) {
  if (a.size() != b.size() || a.size() < 2) {
    throw std::invalid_argument("Vectors must have the same dimension >= 2");
  }
  const T norm_sq_a = a * a;
  const T norm_sq_b = b * b;
  const T dot_prod = a * b;
  const T val = norm_sq_a * norm_sq_b - dot_prod * dot_prod;

  if (val <= static_cast<T>(0)) {
    return static_cast<T>(0);
  }
  return static_cast<T>(0.5) * std::sqrt(val);
}

template float calculate_triangle_area<float>(const Vector<float>&, const Vector<float>&);
template double calculate_triangle_area<double>(const Vector<double>&, const Vector<double>&);
