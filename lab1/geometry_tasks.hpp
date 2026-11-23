#ifndef GEOMETRY_TASKS_HPP_
#define GEOMETRY_TASKS_HPP_

#include <concepts>
#include "vector.hpp"

template <std::floating_point T>
T calculate_triangle_area(const Vector<T>& a, const Vector<T>& b);

#endif
