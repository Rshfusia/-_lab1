#include <complex>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>
#include "geometry_tasks.hpp"
#include "vector.hpp"

int main() {
  std::cout << std::fixed << std::setprecision(4);

  Vector<int> vi(5, -10, 10);
  Vector<float> vf(4, -5.0f, 5.0f);
  Vector<double> vd(4, 0.0, 1.0);
  Vector<std::complex<float>> vcf(3, -2.0f, 2.0f);
  Vector<std::complex<double>> vcd(3, -2.0, 2.0);

  std::cout << "int:            " << vi << "\n";
  std::cout << "float:          " << vf << "\n";
  std::cout << "double:         " << vd << "\n";
  std::cout << "complex float:  " << vcf << "\n";
  std::cout << "complex double: " << vcd << "\n\n";

  Vector<double> a(3, -5.0, 5.0);
  Vector<double> b(3, -5.0, 5.0);

  std::cout << "a (size = " << a.size() << "): " << a << "\n";
  std::cout << "b (size = " << b.size() << "): " << b << "\n";

  a[0] = 3.0;
  std::cout << "a after a[0] = 3.0: " << a << "\n";
  std::cout << "a + b: " << (a + b) << "\n";
  std::cout << "a - b: " << (a - b) << "\n";
  std::cout << "a * b: " << (a * b) << "\n";

  const double scalar = 2.5;
  std::cout << "a * 2.5: " << (a * scalar) << "\n";
  std::cout << "2.5 * a: " << (scalar * a) << "\n";
  std::cout << "a / 2.5: " << (a / scalar) << "\n";

  Vector<double> a_copy(a);
  Vector<double> assigned(1, 0.0);
  assigned = b;
  Vector<double> moved(std::move(assigned));

  std::cout << std::boolalpha;
  std::cout << "a == a_copy: " << (a == a_copy) << "\n";
  std::cout << "a != b:      " << (a != b) << "\n";
  std::cout << "moved b:     " << moved << "\n\n";

  Vector<std::complex<double>> c1(2, std::complex<double>{0.0, 0.0});
  Vector<std::complex<double>> c2(2, std::complex<double>{0.0, 0.0});
  c1[0] = {1.0, 2.0};
  c1[1] = {3.0, 4.0};
  c2[0] = {2.0, -1.0};
  c2[1] = {1.0, 1.0};

  std::cout << "c1 * c2:  " << (c1 * c2) << "\n";
  std::cout << "vcd * vcd: " << (vcd * vcd) << "\n\n";

  try {
    std::cout << a[100] << "\n";
  } catch (const std::out_of_range& e) {
    std::cout << "Caught: " << e.what() << "\n";
  }

  try {
    Vector<double> diff_dim(4, 1.0);
    std::cout << (a + diff_dim) << "\n";
  } catch (const std::invalid_argument& e) {
    std::cout << "Caught: " << e.what() << "\n";
  }

  try {
    std::cout << (a / 0.0) << "\n";
  } catch (const std::invalid_argument& e) {
    std::cout << "Caught: " << e.what() << "\n";
  }

  Vector<double> side1(2, 0.0);
  Vector<double> side2(2, 0.0);
  side1[0] = 4.0;
  side1[1] = 0.0;
  side2[0] = 0.0;
  side2[1] = 3.0;

  double area = calculate_triangle_area(side1, side2);
  std::cout << "\nTriangle area (sides 4 and 3): " << area << "\n";
  std::cout << "Triangle area (random a and b): " << calculate_triangle_area(a, b) << "\n";

  return 0;
}
