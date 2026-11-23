#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cmath>
#include <complex>
#include <cstddef>
#include <iostream>
#include <random>
#include <stdexcept>
#include <type_traits>

template <typename T>
struct IsComplex : std::false_type {};

template <typename T>
struct IsComplex<std::complex<T>> : std::true_type {};

template <typename T>
concept VectorElement = std::is_arithmetic_v<T> || IsComplex<T>::value;

template <typename T>
struct ComponentType {
  using type = T;
};

template <typename T>
struct ComponentType<std::complex<T>> {
  using type = T;
};

template <typename T>
T dot_multiply(const T& a, const T& b) {
  return a * b;
}

template <typename T>
std::complex<T> dot_multiply(const std::complex<T>& a, const std::complex<T>& b) {
  return std::conj(a) * b;
}

template <VectorElement T>
class Vector {
 public:
  using BoundType = typename ComponentType<T>::type;
  static constexpr double kEpsilon = 1e-7;

  Vector(std::size_t size, const T& fill_value)
      : size_(size), data_(size > 0 ? new T[size] : nullptr) {
    for (std::size_t i = 0; i < size_; ++i) {
      data_[i] = fill_value;
    }
  }

  Vector(std::size_t size, BoundType lower_bound, BoundType upper_bound) {
    if (lower_bound > upper_bound) {
      throw std::invalid_argument("Lower bound cannot be greater than upper bound");
    }
    size_ = size;
    data_ = size > 0 ? new T[size] : nullptr;
    fill_random(lower_bound, upper_bound);
  }

  ~Vector() {
    delete[] data_;
  }

  Vector(const Vector& other)
      : size_(other.size_), data_(other.size_ > 0 ? new T[other.size_] : nullptr) {
    for (std::size_t i = 0; i < size_; ++i) {
      data_[i] = other.data_[i];
    }
  }

  Vector& operator=(const Vector& other) {
    if (this != &other) {
      T* new_data = other.size_ > 0 ? new T[other.size_] : nullptr;
      for (std::size_t i = 0; i < other.size_; ++i) {
        new_data[i] = other.data_[i];
      }
      delete[] data_;
      data_ = new_data;
      size_ = other.size_;
    }
    return *this;
  }

  Vector(Vector&& other) noexcept : size_(other.size_), data_(other.data_) {
    other.size_ = 0;
    other.data_ = nullptr;
  }

  Vector& operator=(Vector&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      size_ = other.size_;
      data_ = other.data_;
      other.size_ = 0;
      other.data_ = nullptr;
    }
    return *this;
  }

  std::size_t size() const noexcept {
    return size_;
  }

  T& operator[](std::size_t index) {
    if (index >= size_) {
      throw std::out_of_range("Vector index out of range");
    }
    return data_[index];
  }

  const T& operator[](std::size_t index) const {
    if (index >= size_) {
      throw std::out_of_range("Vector index out of range");
    }
    return data_[index];
  }

  Vector operator+(const Vector& other) const {
    validate_same_dimension(other);
    Vector result(size_, T{});
    for (std::size_t i = 0; i < size_; ++i) {
      result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
  }

  Vector operator-(const Vector& other) const {
    validate_same_dimension(other);
    Vector result(size_, T{});
    for (std::size_t i = 0; i < size_; ++i) {
      result.data_[i] = data_[i] - other.data_[i];
    }
    return result;
  }

  T operator*(const Vector& other) const {
    validate_same_dimension(other);
    T sum{};
    for (std::size_t i = 0; i < size_; ++i) {
      sum += dot_multiply(data_[i], other.data_[i]);
    }
    return sum;
  }

  Vector operator*(const T& scalar) const {
    Vector result(size_, T{});
    for (std::size_t i = 0; i < size_; ++i) {
      result.data_[i] = data_[i] * scalar;
    }
    return result;
  }

  friend Vector operator*(const T& scalar, const Vector& vec) {
    return vec * scalar;
  }

  Vector operator/(const T& scalar) const {
    if (is_zero(scalar)) {
      throw std::invalid_argument("Division by zero scalar");
    }
    Vector result(size_, T{});
    for (std::size_t i = 0; i < size_; ++i) {
      result.data_[i] = data_[i] / scalar;
    }
    return result;
  }

  bool operator==(const Vector& other) const noexcept {
    if (size_ != other.size_) {
      return false;
    }
    for (std::size_t i = 0; i < size_; ++i) {
      if (!are_equal(data_[i], other.data_[i])) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const Vector& other) const noexcept {
    return !(*this == other);
  }

 private:
  std::size_t size_{0};
  T* data_{nullptr};

  void validate_same_dimension(const Vector& other) const {
    if (size_ != other.size_) {
      throw std::invalid_argument("Vector dimensions do not match");
    }
  }

  static bool are_equal(const T& a, const T& b) noexcept {
    if constexpr (std::is_integral_v<T>) {
      return a == b;
    } else if constexpr (std::is_floating_point_v<T>) {
      return std::abs(a - b) <= static_cast<T>(kEpsilon);
    } else if constexpr (IsComplex<T>::value) {
      return std::abs(a.real() - b.real()) <= static_cast<BoundType>(kEpsilon) &&
             std::abs(a.imag() - b.imag()) <= static_cast<BoundType>(kEpsilon);
    }
  }

  static bool is_zero(const T& value) noexcept {
    return are_equal(value, T{});
  }

  void fill_random(BoundType lower_bound, BoundType upper_bound) {
    std::random_device rd;
    std::mt19937_64 gen(rd());

    if constexpr (std::is_integral_v<T>) {
      std::uniform_int_distribution<T> dist(lower_bound, upper_bound);
      for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = dist(gen);
      }
    } else if constexpr (std::is_floating_point_v<T>) {
      std::uniform_real_distribution<T> dist(lower_bound, upper_bound);
      for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = dist(gen);
      }
    } else if constexpr (IsComplex<T>::value) {
      std::uniform_real_distribution<BoundType> dist(lower_bound, upper_bound);
      for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = T(dist(gen), dist(gen));
      }
    }
  }
};

template <VectorElement T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
  os << "[";
  for (std::size_t i = 0; i < vec.size(); ++i) {
    os << vec[i];
    if (i + 1 < vec.size()) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}

#endif
